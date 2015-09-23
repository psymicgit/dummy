///<------------------------------------------------------------------------------
//< @file:   src\gateserver\test.cpp
//< @author: 洪坤安
//< @date:   2015年8月4日 20:56:20
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "test.h"

#include <tool/ticktool.h>
#include <tool/sockettool.h>
#include <tool/atomictool.h>

#include <protocol/message.h>
#include <client.pb.h>

#include <basic/lock.h>
#include <basic/taskqueue.h>

#include <basic/evbuffer.h>
#include <basic/evbuffer-internal.h>

#define TT_STMT_BEGIN do {
#define TT_STMT_END } while (0)
#define EVBUFFER_DATA(x)	evbuffer_pullup((x), -1)

#define evbuffer_validate(buf)			\
	TT_STMT_BEGIN if (!evbuffer_validate_(buf)) assert(("Buffer format invalid\n")); TT_STMT_END

#define  tt_str_op(x, y, z) assert(std::string((char*)x) y std::string(z))
#define  tt_int_op(x, y, z) assert(x y z)
#define  tt_uint_op(x, y, z) assert(x y z)
#define  tt_ptr_op(x, y, z) assert(x y z)

static int evbuffer_validate_(struct evbuffer *buf)
{
	struct evbuffer_chain *chain;
	size_t sum = 0;
	int found_last_with_datap = 0;

	if (buf->first == NULL) {
		assert(buf->last == NULL);
		assert(buf->total_len == 0);
	}

	chain = buf->first;

	assert(buf->last_with_datap);
	if (buf->last_with_datap == &buf->first)
		found_last_with_datap = 1;

	while (chain != NULL) {
		if (&chain->next == buf->last_with_datap)
			found_last_with_datap = 1;
		sum += chain->off;
		if (chain->next == NULL) {
			assert(buf->last == chain);
		}
		assert(chain->buffer_len >= chain->misalign + chain->off);
		chain = chain->next;
	}

	if (buf->first)
		assert(*buf->last_with_datap);

	if (*buf->last_with_datap) {
		chain = *buf->last_with_datap;
		if (chain->off == 0 || buf->total_len == 0) {
			assert(chain->off == 0);
			assert(chain == buf->first);
			assert(buf->total_len == 0);
		}
		chain = chain->next;
		while (chain != NULL) {
			assert(chain->off == 0);
			chain = chain->next;
		}
	} else {
		assert(buf->last_with_datap == &buf->first);
	}
	assert(found_last_with_datap);

	assert(sum == buf->total_len);
	return 1;

end:
	return 0;
}

static void evbuffer_get_waste(struct evbuffer *buf, size_t *allocatedp, size_t *wastedp, size_t *usedp)
{
	struct evbuffer_chain *chain;
	size_t a, w, u;
	int n = 0;
	u = a = w = 0;

	chain = buf->first;
	/* skip empty at start */
	while (chain && chain->off==0) {
		++n;
		a += chain->buffer_len;
		chain = chain->next;
	}
	/* first nonempty chain: stuff at the end only is wasted. */
	if (chain) {
		++n;
		a += chain->buffer_len;
		u += chain->off;
		if (chain->next && chain->next->off)
			w += (size_t)(chain->buffer_len - (chain->misalign + chain->off));
		chain = chain->next;
	}
	/* subsequent nonempty chains */
	while (chain && chain->off) {
		++n;
		a += chain->buffer_len;
		w += (size_t)chain->misalign;
		u += chain->off;
		if (chain->next && chain->next->off)
			w += (size_t) (chain->buffer_len - (chain->misalign + chain->off));
		chain = chain->next;
	}
	/* subsequent empty chains */
	while (chain) {
		++n;
		a += chain->buffer_len;
	}
	*allocatedp = a;
	*wastedp = w;
	*usedp = u;
}

static void test_evbuffer()
{
	static char buffer[512], *tmp;
	struct evbuffer *evb = evbuffer_new();
	struct evbuffer *evb_two = evbuffer_new();
	size_t sz_tmp;
	int i;

	evbuffer_validate(evb);
	evbuffer_add_printf(evb, "%s/%d", "hello", 1);
	evbuffer_validate(evb);

	assert(evbuffer_get_length(evb) == 7);
	assert(!memcmp((char*)EVBUFFER_DATA(evb), "hello/1", 1));

	evbuffer_add_buffer(evb, evb_two);
	evbuffer_validate(evb);

	evbuffer_drain(evb, strlen("hello/"));
	evbuffer_validate(evb);
	assert(evbuffer_get_length(evb) == 1);
	assert(!memcmp((char*)EVBUFFER_DATA(evb), "1", 1));

	evbuffer_add_printf(evb_two, "%s", "/hello");
	evbuffer_validate(evb);
	evbuffer_add_buffer(evb, evb_two);
	evbuffer_validate(evb);

	assert(evbuffer_get_length(evb_two) == 0);
	assert(evbuffer_get_length(evb) == 7);
	assert(!memcmp((char*)EVBUFFER_DATA(evb), "1/hello", 7) != 0);

	memset(buffer, 0, sizeof(buffer));
	evbuffer_add(evb, buffer, sizeof(buffer));
	evbuffer_validate(evb);
	assert(evbuffer_get_length(evb) == 7 + 512);

	tmp = (char *)evbuffer_pullup(evb, 7 + 512);
	assert(tmp);
	assert(!strncmp(tmp, "1/hello", 7));
	assert(!memcmp(tmp + 7, buffer, sizeof(buffer)));
	evbuffer_validate(evb);

	evbuffer_prepend(evb, "something", 9);
	evbuffer_validate(evb);
	evbuffer_prepend(evb, "else", 4);
	evbuffer_validate(evb);

	tmp = (char *)evbuffer_pullup(evb, 4 + 9 + 7);
	assert(!strncmp(tmp, "elsesomething1/hello", 4 + 9 + 7));
	evbuffer_validate(evb);

	evbuffer_drain(evb, -1);
	evbuffer_validate(evb);
	evbuffer_drain(evb_two, -1);
	evbuffer_validate(evb);

	for (i = 0; i < 3; ++i) {
		evbuffer_add(evb_two, buffer, sizeof(buffer));
		evbuffer_validate(evb_two);
		evbuffer_add_buffer(evb, evb_two);
		evbuffer_validate(evb);
		evbuffer_validate(evb_two);
	}

	assert(evbuffer_get_length(evb_two) == 0);
	assert(evbuffer_get_length(evb) == i * sizeof(buffer));

	/* test remove buffer */
	sz_tmp = (size_t)(sizeof(buffer)*2.5);
	evbuffer_remove_buffer(evb, evb_two, sz_tmp);
	assert(evbuffer_get_length(evb_two) == sz_tmp);
	assert(evbuffer_get_length(evb) == sizeof(buffer) / 2);
	evbuffer_validate(evb);

	if (memcmp(evbuffer_pullup(
	               evb, -1), buffer, sizeof(buffer) / 2) != 0 ||
	        memcmp(evbuffer_pullup(
	                   evb_two, -1), buffer, sizeof(buffer)) != 0)
		assert("Pullup did not preserve content");

	evbuffer_validate(evb);


	/* testing one-vector reserve and commit */
	{
		struct evbuffer_iovec v[1];
		char *buf;
		int i, j, r;

		for (i = 0; i < 3; ++i) {
			r = evbuffer_reserve_space(evb, 10000, v, 1);
			assert(r  == 1);
			assert(v[0].iov_len >= 10000);
			assert(v[0].iov_base != NULL);

			evbuffer_validate(evb);
			buf = (char*)v[0].iov_base;
			for (j = 0; j < 10000; ++j) {
				buf[j] = j;
			}
			evbuffer_validate(evb);

			assert(evbuffer_commit_space(evb, v, 1) == 0);
			evbuffer_validate(evb);

			assert(evbuffer_get_length(evb) >= 10000);

			evbuffer_drain(evb, j * 5000);
			evbuffer_validate(evb);
		}
	}

end:
	evbuffer_free(evb);
	evbuffer_free(evb_two);
}

static void
no_cleanup(const void *data, size_t datalen, void *extra)
{
}

static void
test_evbuffer_remove_buffer_with_empty()
{
	struct evbuffer *src = evbuffer_new();
	struct evbuffer *dst = evbuffer_new();
	char buf[2];

	evbuffer_validate(src);
	evbuffer_validate(dst);

	/* setup the buffers */
	/* we need more data in src than we will move later */
	evbuffer_add_reference(src, buf, sizeof(buf), no_cleanup, NULL);
	evbuffer_add_reference(src, buf, sizeof(buf), no_cleanup, NULL);
	/* we need one buffer in dst and one empty buffer at the end */
	evbuffer_add(dst, buf, sizeof(buf));
	evbuffer_add_reference(dst, buf, 0, no_cleanup, NULL);

	evbuffer_validate(src);
	evbuffer_validate(dst);

	/* move three bytes over */
	evbuffer_remove_buffer(src, dst, 3);

	evbuffer_validate(src);
	evbuffer_validate(dst);

end:
	evbuffer_free(src);
	evbuffer_free(dst);
}

static void
test_evbuffer_reserve2(void *ptr)
{
	/* Test the two-vector cases of reserve/commit. */
	struct evbuffer *buf = evbuffer_new();
	int n, i;
	struct evbuffer_iovec v[2];
	size_t remaining;
	char *cp, *cp2;

	/* First chunk will necessarily be one chunk. Use 512 bytes of it.*/
	n = evbuffer_reserve_space(buf, 1024, v, 2);
	tt_int_op(n, ==, 1);
	tt_int_op(evbuffer_get_length(buf), ==, 0);
	assert(v[0].iov_base != NULL);
	tt_int_op(v[0].iov_len, >=, 1024);
	memset(v[0].iov_base, 'X', 512);
	cp = (char*)v[0].iov_base;
	remaining = v[0].iov_len - 512;
	v[0].iov_len = 512;
	evbuffer_validate(buf);
	tt_int_op(0, ==, evbuffer_commit_space(buf, v, 1));
	tt_int_op(evbuffer_get_length(buf), ==, 512);
	evbuffer_validate(buf);

	/* Ask for another same-chunk request, in an existing chunk. Use 8
	 * bytes of it. */
	n = evbuffer_reserve_space(buf, 32, v, 2);
	tt_int_op(n, ==, 1);
	assert(cp + 512 == v[0].iov_base);
	tt_int_op(remaining, ==, v[0].iov_len);
	memset(v[0].iov_base, 'Y', 8);
	v[0].iov_len = 8;
	tt_int_op(0, ==, evbuffer_commit_space(buf, v, 1));
	tt_int_op(evbuffer_get_length(buf), ==, 520);
	remaining -= 8;
	evbuffer_validate(buf);

	/* Now ask for a request that will be split. Use only one byte of it,
	   though. */
	n = evbuffer_reserve_space(buf, remaining+64, v, 2);
	tt_int_op(n, ==, 2);
	assert(cp + 520 == v[0].iov_base);
	tt_int_op(remaining, ==, v[0].iov_len);
	assert(v[1].iov_base);
	assert(v[1].iov_len >= 64);
	cp2 = (char*)v[1].iov_base;
	memset(v[0].iov_base, 'Z', 1);
	v[0].iov_len = 1;
	tt_int_op(0, ==, evbuffer_commit_space(buf, v, 1));
	tt_int_op(evbuffer_get_length(buf), ==, 521);
	remaining -= 1;
	evbuffer_validate(buf);

	/* Now ask for a request that will be split. Use some of the first
	 * part and some of the second. */
	n = evbuffer_reserve_space(buf, remaining+64, v, 2);
	evbuffer_validate(buf);
	tt_int_op(n, ==, 2);
	assert(cp + 521 == v[0].iov_base);
	tt_int_op(remaining, ==, v[0].iov_len);
	assert(v[1].iov_base == cp2);
	assert(v[1].iov_len >= 64);
	memset(v[0].iov_base, 'W', 400);
	v[0].iov_len = 400;
	memset(v[1].iov_base, 'x', 60);
	v[1].iov_len = 60;
	tt_int_op(0, ==, evbuffer_commit_space(buf, v, 2));
	tt_int_op(evbuffer_get_length(buf), ==, 981);
	evbuffer_validate(buf);

	/* Now peek to make sure stuff got made how we like. */
	memset(v,0,sizeof(v));
	n = evbuffer_peek(buf, -1, NULL, v, 2);
	tt_int_op(n, ==, 2);
	tt_int_op(v[0].iov_len, ==, 921);
	tt_int_op(v[1].iov_len, ==, 60);

	cp = (char*)v[0].iov_base;
	for (i=0; i<512; ++i)
		tt_int_op(cp[i], ==, 'X');
	for (i=512; i<520; ++i)
		tt_int_op(cp[i], ==, 'Y');
	for (i=520; i<521; ++i)
		tt_int_op(cp[i], ==, 'Z');
	for (i=521; i<921; ++i)
		tt_int_op(cp[i], ==, 'W');

	cp = (char*)v[1].iov_base;
	for (i=0; i<60; ++i)
		tt_int_op(cp[i], ==, 'x');

end:
	evbuffer_free(buf);
}

static void
test_evbuffer_reserve_many(void *ptr)
{
	/* This is a glass-box test to handle expanding a buffer with more
	 * chunks and reallocating chunks as needed */
	struct evbuffer *buf = evbuffer_new();
	struct evbuffer_iovec v[8];
	int n;
	size_t sz;
	int add_data = ptr && !strcmp((char*)ptr, "add");
	int fill_first = ptr && !strcmp((char*)ptr, "fill");
	char *cp1, *cp2;

	/* When reserving the the first chunk, we just allocate it */
	n = evbuffer_reserve_space(buf, 128, v, 2);
	evbuffer_validate(buf);
	tt_int_op(n, ==, 1);
	assert(v[0].iov_len >= 128);
	sz = v[0].iov_len;
	cp1 = (char*)v[0].iov_base;
	if (add_data) {
		*(char*)v[0].iov_base = 'X';
		v[0].iov_len = 1;
		n = evbuffer_commit_space(buf, v, 1);
		tt_int_op(n, ==, 0);
	} else if (fill_first) {
		memset(v[0].iov_base, 'X', v[0].iov_len);
		n = evbuffer_commit_space(buf, v, 1);
		tt_int_op(n, ==, 0);
		n = evbuffer_reserve_space(buf, 128, v, 2);
		tt_int_op(n, ==, 1);
		sz = v[0].iov_len;
		assert(v[0].iov_base != cp1);
		cp1 = (char*)v[0].iov_base;
	}

	/* Make another chunk get added. */
	n = evbuffer_reserve_space(buf, sz+128, v, 2);
	evbuffer_validate(buf);
	tt_int_op(n, ==, 2);
	sz = v[0].iov_len + v[1].iov_len;
	tt_int_op(sz, >=, v[0].iov_len+128);
	if (add_data) {
		assert(v[0].iov_base == cp1 + 1);
	} else {
		assert(v[0].iov_base == cp1);
	}
	cp1 = (char*)v[0].iov_base;
	cp2 = (char*)v[1].iov_base;

	/* And a third chunk. */
	n = evbuffer_reserve_space(buf, sz+128, v, 3);
	evbuffer_validate(buf);
	tt_int_op(n, ==, 3);
	assert(cp1 == v[0].iov_base);
	assert(cp2 == v[1].iov_base);
	sz = v[0].iov_len + v[1].iov_len + v[2].iov_len;

	/* Now force a reallocation by asking for more space in only 2
	 * buffers. */
	n = evbuffer_reserve_space(buf, sz+128, v, 2);
	evbuffer_validate(buf);
	if (add_data) {
		tt_int_op(n, ==, 2);
		assert(cp1 == v[0].iov_base);
	} else {
		tt_int_op(n, ==, 1);
	}

end:
	evbuffer_free(buf);
}

static void
test_evbuffer_expand(void *ptr)
{
	char data[4096];
	struct evbuffer *buf;
	size_t a,w,u;
	void *buffer;

	memset(data, 'X', sizeof(data));

	/* Make sure that expand() works on an empty buffer */
	buf = evbuffer_new();
	tt_int_op(evbuffer_expand(buf, 20000), ==, 0);
	evbuffer_validate(buf);
	a=w=u=0;
	evbuffer_get_waste(buf, &a,&w,&u);
	assert(w == 0);
	assert(u == 0);
	assert(a >= 20000);
	assert(buf->first);
	assert(buf->first == buf->last);
	assert(buf->first->off == 0);
	assert(buf->first->buffer_len >= 20000);

	/* Make sure that expand() works as a no-op when there's enough
	 * contiguous space already. */
	buffer = buf->first->buffer;
	evbuffer_add(buf, data, 1024);
	tt_int_op(evbuffer_expand(buf, 1024), ==, 0);
	assert(buf->first->buffer == buffer);
	evbuffer_validate(buf);
	evbuffer_free(buf);

	/* Make sure that expand() can work by moving misaligned data
	 * when it makes sense to do so. */
	buf = evbuffer_new();
	evbuffer_add(buf, data, 400);
	{
		int n = (int)(buf->first->buffer_len - buf->first->off - 1);
		assert(n < (int)sizeof(data));
		evbuffer_add(buf, data, n);
	}
	assert(buf->first == buf->last);
	assert(buf->first->off == buf->first->buffer_len - 1);
	evbuffer_drain(buf, buf->first->off - 1);
	assert(1 == evbuffer_get_length(buf));
	assert(buf->first->misalign > 0);
	assert(buf->first->off == 1);
	buffer = buf->first->buffer;
	assert(evbuffer_expand(buf, 40) == 0);
	assert(buf->first == buf->last);
	assert(buf->first->off == 1);
	assert(buf->first->buffer == buffer);
	assert(buf->first->misalign == 0);
	evbuffer_validate(buf);
	evbuffer_free(buf);

	/* add, expand, pull-up: This used to crash libevent. */
	buf = evbuffer_new();

	evbuffer_add(buf, data, sizeof(data));
	evbuffer_add(buf, data, sizeof(data));
	evbuffer_add(buf, data, sizeof(data));

	evbuffer_validate(buf);
	evbuffer_expand(buf, 1024);
	evbuffer_validate(buf);
	evbuffer_pullup(buf, -1);
	evbuffer_validate(buf);

end:
	evbuffer_free(buf);
}


static int reference_cb_called;
static void
reference_cb(const void *data, size_t len, void *extra)
{
	tt_str_op(data, ==, "this is what we add as read-only memory.");
	tt_int_op(len, ==, strlen((char*)data));
	assert(extra == (void *)0xdeadaffe);
	++reference_cb_called;
end:
	;
}

static void
test_evbuffer_reference(void *ptr)
{
	struct evbuffer *src = evbuffer_new();
	struct evbuffer *dst = evbuffer_new();
	struct evbuffer_iovec v[1];
	const char *data = "this is what we add as read-only memory.";
	reference_cb_called = 0;

	assert(evbuffer_add_reference(src, data, strlen(data),
	                              reference_cb, (void *)0xdeadaffe) != -1);

	evbuffer_reserve_space(dst, strlen(data), v, 1);
	assert(evbuffer_remove(src, v[0].iov_base, 10) != -1);

	evbuffer_validate(src);
	evbuffer_validate(dst);

	/* make sure that we don't write data at the beginning */
	evbuffer_prepend(src, "aaaaa", 5);
	evbuffer_validate(src);
	evbuffer_drain(src, 5);

	assert(evbuffer_remove(src, ((char*)(v[0].iov_base)) + 10,
	                       strlen(data) - 10) != -1);

	v[0].iov_len = strlen(data);

	evbuffer_commit_space(dst, v, 1);
	evbuffer_validate(src);
	evbuffer_validate(dst);

	tt_int_op(reference_cb_called, ==, 1);

	assert(!memcmp(evbuffer_pullup(dst, strlen(data)),
	               data, strlen(data)));
	evbuffer_validate(dst);

end:
	evbuffer_free(dst);
	evbuffer_free(src);
}

static struct event_base *addfile_test_event_base = NULL;
static int addfile_test_done_writing = 0;
static int addfile_test_total_written = 0;
static int addfile_test_total_read = 0;

#ifndef EVENT__DISABLE_MM_REPLACEMENT
static void *
failing_malloc(size_t how_much)
{
	errno = ENOMEM;
	return NULL;
}
#endif

static void
test_evbuffer_readln(void *ptr)
{
	struct evbuffer *evb = evbuffer_new();
	struct evbuffer *evb_tmp = evbuffer_new();
	const char *s;
	char *cp = NULL;
	size_t sz;

#define tt_line_eq(content)						\
	TT_STMT_BEGIN							\
	if (!cp || sz != strlen(content) || strcmp(cp, content)) {	\
		printf("Wanted %s; got %s [%d]", content, cp, (int)sz); \
	}								\
	TT_STMT_END

	/* Test EOL_ANY. */
	s = "complex silly newline\r\n\n\r\n\n\rmore\0\n";
	evbuffer_add(evb, s, strlen(s)+2);
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_ANY);
	tt_line_eq("complex silly newline");
	free(cp);
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_ANY);
	if (!cp || sz != 5 || memcmp(cp, "more\0\0", 6))
		printf("Not as expected");
	tt_uint_op(evbuffer_get_length(evb), ==, 0);
	evbuffer_validate(evb);
	s = "\nno newline";
	evbuffer_add(evb, s, strlen(s));
	free(cp);
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_ANY);
	tt_line_eq("");
	free(cp);
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_ANY);
	assert(!cp);
	evbuffer_validate(evb);
	evbuffer_drain(evb, evbuffer_get_length(evb));
	assert(evbuffer_get_length(evb) == 0);
	evbuffer_validate(evb);

	/* Test EOL_CRLF */
	s = "Line with\rin the middle\nLine with good crlf\r\n\nfinal\n";
	evbuffer_add(evb, s, strlen(s));
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF);
	tt_line_eq("Line with\rin the middle");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF);
	tt_line_eq("Line with good crlf");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF);
	tt_line_eq("");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF);
	tt_line_eq("final");
	s = "x";
	evbuffer_validate(evb);
	evbuffer_add(evb, s, 1);
	evbuffer_validate(evb);
	free(cp);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF);
	assert(!cp);
	evbuffer_validate(evb);

	/* Test CRLF_STRICT */
	s = " and a bad crlf\nand a good one\r\n\r\nMore\r";
	evbuffer_add(evb, s, strlen(s));
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	tt_line_eq("x and a bad crlf\nand a good one");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	tt_line_eq("");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	assert(!cp);
	evbuffer_validate(evb);
	evbuffer_add(evb, "\n", 1);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	tt_line_eq("More");
	free(cp);
	assert(evbuffer_get_length(evb) == 0);
	evbuffer_validate(evb);

	s = "An internal CR\r is not an eol\r\nNor is a lack of one";
	evbuffer_add(evb, s, strlen(s));
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	tt_line_eq("An internal CR\r is not an eol");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	assert(!cp);
	evbuffer_validate(evb);

	evbuffer_add(evb, "\r\n", 2);
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	tt_line_eq("Nor is a lack of one");
	free(cp);
	assert(evbuffer_get_length(evb) == 0);
	evbuffer_validate(evb);

	/* Test LF */
	s = "An\rand a nl\n\nText";
	evbuffer_add(evb, s, strlen(s));
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_LF);
	tt_line_eq("An\rand a nl");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_LF);
	tt_line_eq("");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_LF);
	assert(!cp);
	free(cp);
	evbuffer_add(evb, "\n", 1);
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_LF);
	tt_line_eq("Text");
	free(cp);
	evbuffer_validate(evb);

	/* Test NUL */
	tt_int_op(evbuffer_get_length(evb), ==, 0);
	{
		char x[] =
		    "NUL\n\0\0"
		    "The all-zeros character which may serve\0"
		    "to accomplish time fill\0and media fill";
		/* Add all but the final NUL of x. */
		evbuffer_add(evb, x, sizeof(x)-1);
	}
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_NUL);
	tt_line_eq("NUL\n");
	free(cp);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_NUL);
	tt_line_eq("");
	free(cp);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_NUL);
	tt_line_eq("The all-zeros character which may serve");
	free(cp);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_NUL);
	tt_line_eq("to accomplish time fill");
	free(cp);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_NUL);
	tt_ptr_op(cp, ==, NULL);
	evbuffer_drain(evb, -1);

	/* Test CRLF_STRICT - across boundaries*/
	s = " and a bad crlf\nand a good one\r";
	evbuffer_add(evb_tmp, s, strlen(s));
	evbuffer_validate(evb);
	evbuffer_add_buffer(evb, evb_tmp);
	evbuffer_validate(evb);
	s = "\n\r";
	evbuffer_add(evb_tmp, s, strlen(s));
	evbuffer_validate(evb);
	evbuffer_add_buffer(evb, evb_tmp);
	evbuffer_validate(evb);
	s = "\nMore\r";
	evbuffer_add(evb_tmp, s, strlen(s));
	evbuffer_validate(evb);
	evbuffer_add_buffer(evb, evb_tmp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	tt_line_eq(" and a bad crlf\nand a good one");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	tt_line_eq("");
	free(cp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	assert(!cp);
	free(cp);
	evbuffer_validate(evb);
	evbuffer_add(evb, "\n", 1);
	evbuffer_validate(evb);
	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_CRLF_STRICT);
	tt_line_eq("More");
	free(cp);
	cp = NULL;
	evbuffer_validate(evb);
	assert(evbuffer_get_length(evb) == 0);

	/* Test memory problem*/
	s = "one line\ntwo line\nblue line";
	evbuffer_add(evb_tmp, s, strlen(s));
	evbuffer_validate(evb);
	evbuffer_add_buffer(evb, evb_tmp);
	evbuffer_validate(evb);

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_LF);
	tt_line_eq("one line");
	free(cp);
	cp = NULL;
	evbuffer_validate(evb);

	/* the next call to readline should fail */

	cp = evbuffer_readln(evb, &sz, EVBUFFER_EOL_LF);
	tt_line_eq("two line");
	free(cp);
	cp = NULL;
	evbuffer_validate(evb);

end:
	evbuffer_free(evb);
	evbuffer_free(evb_tmp);
	if (cp) free(cp);
}

static void
test_evbuffer_search_eol(void *ptr)
{
	struct evbuffer *buf = evbuffer_new();
	struct evbuffer_ptr ptr1, ptr2;
	const char *s;
	size_t eol_len;

	s = "string! \r\n\r\nx\n";
	evbuffer_add(buf, s, strlen(s));
	eol_len = -1;
	ptr1 = evbuffer_search_eol(buf, NULL, &eol_len, EVBUFFER_EOL_CRLF);
	tt_int_op(ptr1.pos, ==, 8);
	tt_int_op(eol_len, ==, 2);

	eol_len = -1;
	ptr2 = evbuffer_search_eol(buf, &ptr1, &eol_len, EVBUFFER_EOL_CRLF);
	tt_int_op(ptr2.pos, ==, 8);
	tt_int_op(eol_len, ==, 2);

	evbuffer_ptr_set(buf, &ptr1, 1, EVBUFFER_PTR_ADD);
	eol_len = -1;
	ptr2 = evbuffer_search_eol(buf, &ptr1, &eol_len, EVBUFFER_EOL_CRLF);
	tt_int_op(ptr2.pos, ==, 9);
	tt_int_op(eol_len, ==, 1);

	eol_len = -1;
	ptr2 = evbuffer_search_eol(buf, &ptr1, &eol_len, EVBUFFER_EOL_CRLF_STRICT);
	tt_int_op(ptr2.pos, ==, 10);
	tt_int_op(eol_len, ==, 2);

	eol_len = -1;
	ptr1 = evbuffer_search_eol(buf, NULL, &eol_len, EVBUFFER_EOL_LF);
	tt_int_op(ptr1.pos, ==, 9);
	tt_int_op(eol_len, ==, 1);

	eol_len = -1;
	ptr2 = evbuffer_search_eol(buf, &ptr1, &eol_len, EVBUFFER_EOL_LF);
	tt_int_op(ptr2.pos, ==, 9);
	tt_int_op(eol_len, ==, 1);

	evbuffer_ptr_set(buf, &ptr1, 1, EVBUFFER_PTR_ADD);
	eol_len = -1;
	ptr2 = evbuffer_search_eol(buf, &ptr1, &eol_len, EVBUFFER_EOL_LF);
	tt_int_op(ptr2.pos, ==, 11);
	tt_int_op(eol_len, ==, 1);

	assert(evbuffer_ptr_set(buf, &ptr1, evbuffer_get_length(buf), EVBUFFER_PTR_SET) == 0);
	eol_len = -1;
	ptr2 = evbuffer_search_eol(buf, &ptr1, &eol_len, EVBUFFER_EOL_LF);
	tt_int_op(ptr2.pos, ==, -1);
	tt_int_op(eol_len, ==, 0);

end:
	evbuffer_free(buf);
}

// 本函数固定是通不过的
static void
test_evbuffer_iterative(void *ptr)
{
	struct evbuffer *buf = evbuffer_new();
	const char *abc = "abcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyz";
	unsigned i, j, sum, n;

	sum = 0;
	n = 0;
	for (i = 0; i < 1000; ++i) {
		for (j = 1; j < strlen(abc); ++j) {
			char format[32];
			sprintf_s(format, sizeof(format), "%%%u.%us", j, j);
			evbuffer_add_printf(buf, format, abc);

			char s[100];
			sprintf_s(s, sizeof(s), format, abc);

			/* Only check for rep violations every so often.
			   Walking over the whole list of chains can get
			   pretty expensive as it gets long.
			 */
			if ((n % 337) == 0)
				evbuffer_validate(buf);

			sum += j;

			tt_uint_op(sum, ==, evbuffer_get_length(buf));

			n++;
		}
	}
	evbuffer_validate(buf);

	tt_uint_op(sum, ==, evbuffer_get_length(buf));

	{
		size_t a,w,u;
		a=w=u=0;
		evbuffer_get_waste(buf, &a, &w, &u);
		if (0)
			printf("Allocated: %u.\nWasted: %u.\nUsed: %u.",
			       (unsigned)a, (unsigned)w, (unsigned)u);
		assert( ((double)w)/a < .125);
	}
end:
	evbuffer_free(buf);

}

static void
test_evbuffer_find(void *ptr)
{
	unsigned char* p;
	const char* test1 = "1234567890\r\n";
	const char* test2 = "1234567890\r";
#define EVBUFFER_INITIAL_LENGTH 256
	char test3[EVBUFFER_INITIAL_LENGTH];
	unsigned int i;
	struct evbuffer * buf = evbuffer_new();

	assert(buf);

	/* make sure evbuffer_find doesn't match past the end of the buffer */
	evbuffer_add(buf, (unsigned char*)test1, strlen(test1));
	evbuffer_validate(buf);
	evbuffer_drain(buf, strlen(test1));
	evbuffer_validate(buf);
	evbuffer_add(buf, (unsigned char*)test2, strlen(test2));
	evbuffer_validate(buf);
	p = evbuffer_find(buf, (unsigned char*)"\r\n", 2);
	assert(p == NULL);

	/*
	 * drain the buffer and do another find; in r309 this would
	 * read past the allocated buffer causing a valgrind error.
	 */
	evbuffer_drain(buf, strlen(test2));
	evbuffer_validate(buf);
	for (i = 0; i < EVBUFFER_INITIAL_LENGTH; ++i)
		test3[i] = 'a';
	test3[EVBUFFER_INITIAL_LENGTH - 1] = 'x';
	evbuffer_add(buf, (unsigned char *)test3, EVBUFFER_INITIAL_LENGTH);
	evbuffer_validate(buf);
	p = evbuffer_find(buf, (unsigned char *)"xy", 2);
	assert(p == NULL);

	/* simple test for match at end of allocated buffer */
	p = evbuffer_find(buf, (unsigned char *)"ax", 2);
	assert(p != NULL);
	assert(strncmp((char*)p, "ax", 2) == 0);

end:
	if (buf)
		evbuffer_free(buf);
}

static void
test_evbuffer_ptr_set(void *ptr)
{
	struct evbuffer *buf = evbuffer_new();
	struct evbuffer_ptr pos;
	struct evbuffer_iovec v[1];

	assert(buf);

	tt_int_op(evbuffer_get_length(buf), ==, 0);

	assert(evbuffer_ptr_set(buf, &pos, 0, EVBUFFER_PTR_SET) == 0);
	assert(pos.pos == 0);
	assert(evbuffer_ptr_set(buf, &pos, 1, EVBUFFER_PTR_ADD) == -1);
	assert(pos.pos == -1);
	assert(evbuffer_ptr_set(buf, &pos, 1, EVBUFFER_PTR_SET) == -1);
	assert(pos.pos == -1);

	/* create some chains */
	evbuffer_reserve_space(buf, 5000, v, 1);
	v[0].iov_len = 5000;
	memset(v[0].iov_base, 1, v[0].iov_len);
	evbuffer_commit_space(buf, v, 1);
	evbuffer_validate(buf);

	evbuffer_reserve_space(buf, 4000, v, 1);
	v[0].iov_len = 4000;
	memset(v[0].iov_base, 2, v[0].iov_len);
	evbuffer_commit_space(buf, v, 1);

	evbuffer_reserve_space(buf, 3000, v, 1);
	v[0].iov_len = 3000;
	memset(v[0].iov_base, 3, v[0].iov_len);
	evbuffer_commit_space(buf, v, 1);
	evbuffer_validate(buf);

	tt_int_op(evbuffer_get_length(buf), ==, 12000);

	assert(evbuffer_ptr_set(buf, &pos, 13000, EVBUFFER_PTR_SET) == -1);
	assert(pos.pos == -1);
	assert(evbuffer_ptr_set(buf, &pos, 0, EVBUFFER_PTR_SET) == 0);
	assert(pos.pos == 0);
	assert(evbuffer_ptr_set(buf, &pos, 13000, EVBUFFER_PTR_ADD) == -1);

	assert(evbuffer_ptr_set(buf, &pos, 0, EVBUFFER_PTR_SET) == 0);
	assert(pos.pos == 0);
	assert(evbuffer_ptr_set(buf, &pos, 10000, EVBUFFER_PTR_ADD) == 0);
	assert(pos.pos == 10000);
	assert(evbuffer_ptr_set(buf, &pos, 1000, EVBUFFER_PTR_ADD) == 0);
	assert(pos.pos == 11000);
	assert(evbuffer_ptr_set(buf, &pos, 1000, EVBUFFER_PTR_ADD) == 0);
	assert(pos.pos == 12000);
	assert(evbuffer_ptr_set(buf, &pos, 1000, EVBUFFER_PTR_ADD) == -1);
	assert(pos.pos == -1);

end:
	if (buf)
		evbuffer_free(buf);
}

static void
test_evbuffer_search(void *ptr)
{
	struct evbuffer *buf = evbuffer_new();
	struct evbuffer *tmp = evbuffer_new();
	struct evbuffer_ptr pos, end;

	assert(buf);
	assert(tmp);

	pos = evbuffer_search(buf, "x", 1, NULL);
	tt_int_op(pos.pos, ==, -1);
	assert(evbuffer_ptr_set(buf, &pos, 0, EVBUFFER_PTR_SET) == 0);
	pos = evbuffer_search(buf, "x", 1, &pos);
	tt_int_op(pos.pos, ==, -1);
	assert(evbuffer_ptr_set(buf, &pos, 0, EVBUFFER_PTR_SET) == 0);
	pos = evbuffer_search_range(buf, "x", 1, &pos, &pos);
	tt_int_op(pos.pos, ==, -1);
	assert(evbuffer_ptr_set(buf, &pos, 0, EVBUFFER_PTR_SET) == 0);
	pos = evbuffer_search_range(buf, "x", 1, &pos, NULL);
	tt_int_op(pos.pos, ==, -1);

	/* set up our chains */
	evbuffer_add_printf(tmp, "hello");  /* 5 chars */
	evbuffer_add_buffer(buf, tmp);
	evbuffer_add_printf(tmp, "foo");    /* 3 chars */
	evbuffer_add_buffer(buf, tmp);
	evbuffer_add_printf(tmp, "cat");    /* 3 chars */
	evbuffer_add_buffer(buf, tmp);
	evbuffer_add_printf(tmp, "attack");
	evbuffer_add_buffer(buf, tmp);

	pos = evbuffer_search(buf, "attack", 6, NULL);
	tt_int_op(pos.pos, ==, 11);
	pos = evbuffer_search(buf, "attacker", 8, NULL);
	tt_int_op(pos.pos, ==, -1);

	/* test continuing search */
	pos = evbuffer_search(buf, "oc", 2, NULL);
	tt_int_op(pos.pos, ==, 7);
	pos = evbuffer_search(buf, "cat", 3, &pos);
	tt_int_op(pos.pos, ==, 8);
	pos = evbuffer_search(buf, "tacking", 7, &pos);
	tt_int_op(pos.pos, ==, -1);

	evbuffer_ptr_set(buf, &pos, 5, EVBUFFER_PTR_SET);
	pos = evbuffer_search(buf, "foo", 3, &pos);
	tt_int_op(pos.pos, ==, 5);

	evbuffer_ptr_set(buf, &pos, 2, EVBUFFER_PTR_ADD);
	pos = evbuffer_search(buf, "tat", 3, &pos);
	tt_int_op(pos.pos, ==, 10);

	/* test bounded search. */
	/* Set "end" to the first t in "attack". */
	evbuffer_ptr_set(buf, &end, 12, EVBUFFER_PTR_SET);
	pos = evbuffer_search_range(buf, "foo", 3, NULL, &end);
	tt_int_op(pos.pos, ==, 5);
	pos = evbuffer_search_range(buf, "foocata", 7, NULL, &end);
	tt_int_op(pos.pos, ==, 5);
	pos = evbuffer_search_range(buf, "foocatat", 8, NULL, &end);
	tt_int_op(pos.pos, ==, -1);
	pos = evbuffer_search_range(buf, "ack", 3, NULL, &end);
	tt_int_op(pos.pos, ==, -1);

	/* Set "end" after the last byte in the buffer. */
	assert(evbuffer_ptr_set(buf, &end, 17, EVBUFFER_PTR_SET) == 0);

	pos = evbuffer_search_range(buf, "attack", 6, NULL, &end);
	tt_int_op(pos.pos, ==, 11);
	assert(evbuffer_ptr_set(buf, &pos, 11, EVBUFFER_PTR_SET) == 0);
	pos = evbuffer_search_range(buf, "attack", 6, &pos, &end);
	tt_int_op(pos.pos, ==, 11);
	assert(evbuffer_ptr_set(buf, &pos, 17, EVBUFFER_PTR_SET) == 0);
	pos = evbuffer_search_range(buf, "attack", 6, &pos, &end);
	tt_int_op(pos.pos, ==, -1);
	assert(evbuffer_ptr_set(buf, &pos, 17, EVBUFFER_PTR_SET) == 0);
	pos = evbuffer_search_range(buf, "attack", 6, &pos, NULL);
	tt_int_op(pos.pos, ==, -1);

end:
	if (buf)
		evbuffer_free(buf);
	if (tmp)
		evbuffer_free(tmp);
}

static void
log_change_callback(struct evbuffer *buffer,
                    const struct evbuffer_cb_info *cbinfo,
                    void *arg)
{

	size_t old_len = cbinfo->orig_size;
	size_t new_len = old_len + cbinfo->n_added - cbinfo->n_deleted;
	struct evbuffer *out = (evbuffer*)arg;
	evbuffer_add_printf(out, "%lu->%lu; ", (unsigned long)old_len,
	                    (unsigned long)new_len);
}

static int ref_done_cb_called_count = 0;
static void *ref_done_cb_called_with = NULL;
static const void *ref_done_cb_called_with_data = NULL;
static size_t ref_done_cb_called_with_len = 0;
static void ref_done_cb(const void *data, size_t len, void *info)
{
	++ref_done_cb_called_count;
	ref_done_cb_called_with = info;
	ref_done_cb_called_with_data = data;
	ref_done_cb_called_with_len = len;
}

static void
test_evbuffer_add_reference(void *ptr)
{
	const char chunk1[] = "If you have found the answer to such a problem";
	const char chunk2[] = "you ought to write it up for publication";
	/* -- Knuth's "Notes on the Exercises" from TAOCP */
	char tmp[16];
	size_t len1 = strlen(chunk1), len2=strlen(chunk2);

	struct evbuffer *buf1 = NULL, *buf2 = NULL;

	buf1 = evbuffer_new();
	assert(buf1);

	evbuffer_add_reference(buf1, chunk1, len1, ref_done_cb, (void*)111);
	evbuffer_add(buf1, ", ", 2);
	evbuffer_add_reference(buf1, chunk2, len2, ref_done_cb, (void*)222);
	tt_int_op(evbuffer_get_length(buf1), ==, len1+len2+2);

	/* Make sure we can drain a little from a reference. */
	tt_int_op(evbuffer_remove(buf1, tmp, 6), ==, 6);
	tt_int_op(memcmp(tmp, "If you", 6), ==, 0);
	tt_int_op(evbuffer_remove(buf1, tmp, 5), ==, 5);
	tt_int_op(memcmp(tmp, " have", 5), ==, 0);

	/* Make sure that prepending does not meddle with immutable data */
	tt_int_op(evbuffer_prepend(buf1, "I have ", 7), ==, 0);
	tt_int_op(memcmp(chunk1, "If you", 6), ==, 0);
	evbuffer_validate(buf1);

	/* Make sure that when the chunk is over, the callback is invoked. */
	evbuffer_drain(buf1, 7); /* Remove prepended stuff. */
	evbuffer_drain(buf1, len1-11-1); /* remove all but one byte of chunk1 */
	tt_int_op(ref_done_cb_called_count, ==, 0);
	evbuffer_remove(buf1, tmp, 1);
	tt_int_op(tmp[0], ==, 'm');
	assert(ref_done_cb_called_with == (void*)111);
	assert(ref_done_cb_called_with_data == chunk1);
	assert(ref_done_cb_called_with_len == len1);
	tt_int_op(ref_done_cb_called_count, ==, 1);
	evbuffer_validate(buf1);

	/* Drain some of the remaining chunk, then add it to another buffer */
	evbuffer_drain(buf1, 6); /* Remove the ", you ". */
	buf2 = evbuffer_new();
	assert(buf2);
	tt_int_op(ref_done_cb_called_count, ==, 1);
	evbuffer_add(buf2, "I ", 2);

	evbuffer_add_buffer(buf2, buf1);
	tt_int_op(ref_done_cb_called_count, ==, 1);
	evbuffer_remove(buf2, tmp, 16);
	tt_int_op(memcmp("I ought to write", tmp, 16), ==, 0);
	evbuffer_drain(buf2, evbuffer_get_length(buf2));
	tt_int_op(ref_done_cb_called_count, ==, 2);
	assert(ref_done_cb_called_with == (void*)222);
	evbuffer_validate(buf2);

	/* Now add more stuff to buf1 and make sure that it gets removed on
	 * free. */
	evbuffer_add(buf1, "You shake and shake the ", 24);
	evbuffer_add_reference(buf1, "ketchup bottle", 14, ref_done_cb,
	                       (void*)3333);
	evbuffer_add(buf1, ". Nothing comes and then a lot'll.", 35);
	evbuffer_free(buf1);
	buf1 = NULL;
	tt_int_op(ref_done_cb_called_count, ==, 3);
	assert(ref_done_cb_called_with == (void*)3333);

end:
	if (buf1)
		evbuffer_free(buf1);
	if (buf2)
		evbuffer_free(buf2);
}

static void
test_evbuffer_multicast(void *ptr)
{
	const char chunk1[] = "If you have found the answer to such a problem";
	const char chunk2[] = "you ought to write it up for publication";
	/* -- Knuth's "Notes on the Exercises" from TAOCP */
	char tmp[16];
	size_t len1 = strlen(chunk1), len2=strlen(chunk2);

	struct evbuffer *buf1 = NULL, *buf2 = NULL;

	buf1 = evbuffer_new();
	assert(buf1);

	evbuffer_add(buf1, chunk1, len1);
	evbuffer_add(buf1, ", ", 2);
	evbuffer_add(buf1, chunk2, len2);
	tt_int_op(evbuffer_get_length(buf1), ==, len1+len2+2);

	buf2 = evbuffer_new();
	assert(buf2);

	tt_int_op(evbuffer_add_buffer_reference(buf2, buf1), ==, 0);
	/* nested references are not allowed */
	tt_int_op(evbuffer_add_buffer_reference(buf2, buf2), ==, -1);
	tt_int_op(evbuffer_add_buffer_reference(buf1, buf2), ==, -1);

	/* both buffers contain the same amount of data */
	tt_int_op(evbuffer_get_length(buf1), ==, evbuffer_get_length(buf1));

	/* Make sure we can drain a little from the first buffer. */
	tt_int_op(evbuffer_remove(buf1, tmp, 6), ==, 6);
	tt_int_op(memcmp(tmp, "If you", 6), ==, 0);
	tt_int_op(evbuffer_remove(buf1, tmp, 5), ==, 5);
	tt_int_op(memcmp(tmp, " have", 5), ==, 0);

	/* Make sure that prepending does not meddle with immutable data */
	tt_int_op(evbuffer_prepend(buf1, "I have ", 7), ==, 0);
	tt_int_op(memcmp(chunk1, "If you", 6), ==, 0);
	evbuffer_validate(buf1);

	/* Make sure we can drain a little from the second buffer. */
	tt_int_op(evbuffer_remove(buf2, tmp, 6), ==, 6);
	tt_int_op(memcmp(tmp, "If you", 6), ==, 0);
	tt_int_op(evbuffer_remove(buf2, tmp, 5), ==, 5);
	tt_int_op(memcmp(tmp, " have", 5), ==, 0);

	/* Make sure that prepending does not meddle with immutable data */
	tt_int_op(evbuffer_prepend(buf2, "I have ", 7), ==, 0);
	tt_int_op(memcmp(chunk1, "If you", 6), ==, 0);
	evbuffer_validate(buf2);

	/* Make sure the data can be read from the second buffer when the first is freed */
	evbuffer_free(buf1);
	buf1 = NULL;

	tt_int_op(evbuffer_remove(buf2, tmp, 6), ==, 6);
	tt_int_op(memcmp(tmp, "I have", 6), ==, 0);

	tt_int_op(evbuffer_remove(buf2, tmp, 6), ==, 6);
	tt_int_op(memcmp(tmp, "  foun", 6), ==, 0);

end:
	if (buf1)
		evbuffer_free(buf1);
	if (buf2)
		evbuffer_free(buf2);
}

static void
test_evbuffer_multicast_drain(void *ptr)
{
	const char chunk1[] = "If you have found the answer to such a problem";
	const char chunk2[] = "you ought to write it up for publication";
	/* -- Knuth's "Notes on the Exercises" from TAOCP */
	size_t len1 = strlen(chunk1), len2=strlen(chunk2);

	struct evbuffer *buf1 = NULL, *buf2 = NULL;

	buf1 = evbuffer_new();
	assert(buf1);

	evbuffer_add(buf1, chunk1, len1);
	evbuffer_add(buf1, ", ", 2);
	evbuffer_add(buf1, chunk2, len2);
	tt_int_op(evbuffer_get_length(buf1), ==, len1+len2+2);

	buf2 = evbuffer_new();
	assert(buf2);

	tt_int_op(evbuffer_add_buffer_reference(buf2, buf1), ==, 0);
	tt_int_op(evbuffer_get_length(buf2), ==, len1+len2+2);
	tt_int_op(evbuffer_drain(buf1, evbuffer_get_length(buf1)), ==, 0);
	tt_int_op(evbuffer_get_length(buf2), ==, len1+len2+2);
	tt_int_op(evbuffer_drain(buf2, evbuffer_get_length(buf2)), ==, 0);
	evbuffer_validate(buf1);
	evbuffer_validate(buf2);

end:
	if (buf1)
		evbuffer_free(buf1);
	if (buf2)
		evbuffer_free(buf2);
}

/* Some cases that we didn't get in test_evbuffer() above, for more coverage. */
static void
test_evbuffer_prepend(void *ptr)
{
	struct evbuffer *buf1 = NULL, *buf2 = NULL;
	char tmp[128];
	int n;

	buf1 = evbuffer_new();
	assert(buf1);

	/* Case 0: The evbuffer is entirely empty. */
	evbuffer_prepend(buf1, "This string has 29 characters", 29);
	evbuffer_validate(buf1);

	/* Case 1: Prepend goes entirely in new chunk. */
	evbuffer_prepend(buf1, "Short.", 6);
	evbuffer_validate(buf1);

	/* Case 2: prepend goes entirely in first chunk. */
	evbuffer_drain(buf1, 6+11);
	evbuffer_prepend(buf1, "it", 2);
	evbuffer_validate(buf1);
	assert(!memcmp(buf1->first->buffer+buf1->first->misalign,
	               "it has", 6));

	/* Case 3: prepend is split over multiple chunks. */
	evbuffer_prepend(buf1, "It is no longer true to say ", 28);
	evbuffer_validate(buf1);
	n = evbuffer_remove(buf1, tmp, sizeof(tmp)-1);
	tt_int_op(n, >=, 0);
	tmp[n]='\0';
	tt_str_op(tmp,==,"It is no longer true to say it has 29 characters");

	buf2 = evbuffer_new();
	assert(buf2);

	/* Case 4: prepend a buffer to an empty buffer. */
	n = 999;
	evbuffer_add_printf(buf1, "Here is string %d. ", n++);
	evbuffer_prepend_buffer(buf2, buf1);
	evbuffer_validate(buf2);

	/* Case 5: prepend a buffer to a nonempty buffer. */
	evbuffer_add_printf(buf1, "Here is string %d. ", n++);
	evbuffer_prepend_buffer(buf2, buf1);
	evbuffer_validate(buf2);
	evbuffer_validate(buf1);
	n = evbuffer_remove(buf2, tmp, sizeof(tmp)-1);
	tt_int_op(n, >=, 0);
	tmp[n]='\0';
	tt_str_op(tmp,==,"Here is string 1000. Here is string 999. ");

end:
	if (buf1)
		evbuffer_free(buf1);
	if (buf2)
		evbuffer_free(buf2);

}

static void
test_evbuffer_peek_first_gt(void *info)
{
	struct evbuffer *buf = NULL, *tmp_buf = NULL;
	struct evbuffer_ptr ptr;
	struct evbuffer_iovec v[2];

	buf = evbuffer_new();
	tmp_buf = evbuffer_new();
	evbuffer_add_printf(tmp_buf, "Contents of chunk 100\n");
	evbuffer_add_buffer(buf, tmp_buf);
	evbuffer_add_printf(tmp_buf, "Contents of chunk 1\n");
	evbuffer_add_buffer(buf, tmp_buf);

	evbuffer_ptr_set(buf, &ptr, 0, EVBUFFER_PTR_SET);

	/** The only case that matters*/
	tt_int_op(evbuffer_peek(buf, -1, &ptr, NULL, 0), ==, 2);
	/** Just in case */
	tt_int_op(evbuffer_peek(buf, -1, &ptr, v, 2), ==, 2);

	evbuffer_ptr_set(buf, &ptr, 20, EVBUFFER_PTR_ADD);
	tt_int_op(evbuffer_peek(buf, -1, &ptr, NULL, 0), ==, 2);
	tt_int_op(evbuffer_peek(buf, -1, &ptr, v, 2), ==, 2);
	tt_int_op(evbuffer_peek(buf, 2, &ptr, NULL, 0), ==, 1);
	tt_int_op(evbuffer_peek(buf, 2, &ptr, v, 2), ==, 1);
	tt_int_op(evbuffer_peek(buf, 3, &ptr, NULL, 0), ==, 2);
	tt_int_op(evbuffer_peek(buf, 3, &ptr, v, 2), ==, 2);

end:
	if (buf)
		evbuffer_free(buf);
	if (tmp_buf)
		evbuffer_free(tmp_buf);
}

static void
test_evbuffer_peek(void *info)
{
	struct evbuffer *buf = NULL, *tmp_buf = NULL;
	int i;
	struct evbuffer_iovec v[20];
	struct evbuffer_ptr ptr;

#define tt_iov_eq(v, s)						\
	tt_int_op((v)->iov_len, ==, strlen(s));			\
	assert(!memcmp((v)->iov_base, (s), strlen(s)))

	/* Let's make a very fragmented buffer. */
	buf = evbuffer_new();
	tmp_buf = evbuffer_new();
	for (i = 0; i < 16; ++i) {
		evbuffer_add_printf(tmp_buf, "Contents of chunk [%d]\n", i);
		evbuffer_add_buffer(buf, tmp_buf);
	}

	/* How many chunks do we need for everything? */
	i = evbuffer_peek(buf, -1, NULL, NULL, 0);
	tt_int_op(i, ==, 16);

	/* Simple peek: get everything. */
	i = evbuffer_peek(buf, -1, NULL, v, 20);
	tt_int_op(i, ==, 16); /* we used only 16 chunks. */
	tt_iov_eq(&v[0], "Contents of chunk [0]\n");
	tt_iov_eq(&v[3], "Contents of chunk [3]\n");
	tt_iov_eq(&v[12], "Contents of chunk [12]\n");
	tt_iov_eq(&v[15], "Contents of chunk [15]\n");

	/* Just get one chunk worth. */
	memset(v, 0, sizeof(v));
	i = evbuffer_peek(buf, -1, NULL, v, 1);
	tt_int_op(i, ==, 1);
	tt_iov_eq(&v[0], "Contents of chunk [0]\n");
	assert(v[1].iov_base == NULL);

	/* Suppose we want at least the first 40 bytes. */
	memset(v, 0, sizeof(v));
	i = evbuffer_peek(buf, 40, NULL, v, 16);
	tt_int_op(i, ==, 2);
	tt_iov_eq(&v[0], "Contents of chunk [0]\n");
	tt_iov_eq(&v[1], "Contents of chunk [1]\n");
	assert(v[2].iov_base == NULL);

	/* How many chunks do we need for 100 bytes? */
	memset(v, 0, sizeof(v));
	i = evbuffer_peek(buf, 100, NULL, NULL, 0);
	tt_int_op(i, ==, 5);
	assert(v[0].iov_base == NULL);

	/* Now we ask for more bytes than we provide chunks for */
	memset(v, 0, sizeof(v));
	i = evbuffer_peek(buf, 60, NULL, v, 1);
	tt_int_op(i, ==, 3);
	tt_iov_eq(&v[0], "Contents of chunk [0]\n");
	assert(v[1].iov_base == NULL);

	/* Now we ask for more bytes than the buffer has. */
	memset(v, 0, sizeof(v));
	i = evbuffer_peek(buf, 65536, NULL, v, 20);
	tt_int_op(i, ==, 16); /* we used only 16 chunks. */
	tt_iov_eq(&v[0], "Contents of chunk [0]\n");
	tt_iov_eq(&v[3], "Contents of chunk [3]\n");
	tt_iov_eq(&v[12], "Contents of chunk [12]\n");
	tt_iov_eq(&v[15], "Contents of chunk [15]\n");
	assert(v[16].iov_base == NULL);

	/* What happens if we try an empty buffer? */
	memset(v, 0, sizeof(v));
	i = evbuffer_peek(tmp_buf, -1, NULL, v, 20);
	tt_int_op(i, ==, 0);
	assert(v[0].iov_base == NULL);
	memset(v, 0, sizeof(v));
	i = evbuffer_peek(tmp_buf, 50, NULL, v, 20);
	tt_int_op(i, ==, 0);
	assert(v[0].iov_base == NULL);

	/* Okay, now time to have fun with pointers. */
	memset(v, 0, sizeof(v));
	evbuffer_ptr_set(buf, &ptr, 30, EVBUFFER_PTR_SET);
	i = evbuffer_peek(buf, 50, &ptr, v, 20);
	tt_int_op(i, ==, 3);
	tt_iov_eq(&v[0], " of chunk [1]\n");
	tt_iov_eq(&v[1], "Contents of chunk [2]\n");
	tt_iov_eq(&v[2], "Contents of chunk [3]\n"); /*more than we asked for*/

	/* advance to the start of another chain. */
	memset(v, 0, sizeof(v));
	evbuffer_ptr_set(buf, &ptr, 14, EVBUFFER_PTR_ADD);
	i = evbuffer_peek(buf, 44, &ptr, v, 20);
	tt_int_op(i, ==, 2);
	tt_iov_eq(&v[0], "Contents of chunk [2]\n");
	tt_iov_eq(&v[1], "Contents of chunk [3]\n"); /*more than we asked for*/

	/* peek at the end of the buffer */
	memset(v, 0, sizeof(v));
	assert(evbuffer_ptr_set(buf, &ptr, evbuffer_get_length(buf), EVBUFFER_PTR_SET) == 0);
	i = evbuffer_peek(buf, 44, &ptr, v, 20);
	tt_int_op(i, ==, 0);
	assert(v[0].iov_base == NULL);

end:
	if (buf)
		evbuffer_free(buf);
	if (tmp_buf)
		evbuffer_free(tmp_buf);
}

/* Check whether evbuffer freezing works right.  This is called twice,
   once with the argument "start" and once with the argument "end".
   When we test "start", we freeze the start of an evbuffer and make sure
   that modifying the start of the buffer doesn't work.  When we test
   "end", we freeze the end of an evbuffer and make sure that modifying
   the end of the buffer doesn't work.
 */
static void
test_evbuffer_freeze(const char *ptr)
{
	struct evbuffer *buf = NULL, *tmp_buf=NULL;
	const char string[] = /* Year's End, Richard Wilbur */
	    "I've known the wind by water banks to shake\n"
	    "The late leaves down, which frozen where they fell\n"
	    "And held in ice as dancers in a spell\n"
	    "Fluttered all winter long into a lake...";
	const int start = !strcmp((char*)ptr, "start");
	char *cp;
	char charbuf[128];
	int r;
	size_t orig_length;
	struct evbuffer_iovec v[1];

	if (!start)
		tt_str_op(ptr, ==, "end");

	buf = evbuffer_new();
	tmp_buf = evbuffer_new();
	assert(tmp_buf);

	evbuffer_add(buf, string, strlen(string));
	evbuffer_freeze(buf, start); /* Freeze the start or the end.*/

#define FREEZE_EQ(a, startcase, endcase)		\
	do {						\
	    if (start) {				\
		    tt_int_op((a), ==, (startcase));	\
	    } else {					\
		    tt_int_op((a), ==, (endcase));	\
	    }						\
	} while (0)


	orig_length = evbuffer_get_length(buf);

	/* These functions all manipulate the end of buf. */
	r = evbuffer_add(buf, "abc", 0);
	FREEZE_EQ(r, 0, -1);
	r = evbuffer_reserve_space(buf, 10, v, 1);
	FREEZE_EQ(r, 1, -1);
	if (r == 1) {
		memset(v[0].iov_base, 'X', 10);
		v[0].iov_len = 10;
	}
	r = evbuffer_commit_space(buf, v, 1);
	FREEZE_EQ(r, 0, -1);
	r = evbuffer_add_reference(buf, string, 5, NULL, NULL);
	FREEZE_EQ(r, 0, -1);
	r = evbuffer_add_printf(buf, "Hello %s", "world");
	FREEZE_EQ(r, 11, -1);
	/* TODO: test add_buffer, add_file, read */

	if (!start)
		tt_int_op(orig_length, ==, evbuffer_get_length(buf));

	orig_length = evbuffer_get_length(buf);

	/* These functions all manipulate the start of buf. */
	r = evbuffer_remove(buf, charbuf, 1);
	FREEZE_EQ(r, -1, 1);
	r = evbuffer_drain(buf, 3);
	FREEZE_EQ(r, -1, 0);
	r = evbuffer_prepend(buf, "dummy", 5);
	FREEZE_EQ(r, -1, 0);
	cp = evbuffer_readln(buf, NULL, EVBUFFER_EOL_LF);
	FREEZE_EQ(cp==NULL, 1, 0);
	if (cp)
		free(cp);
	/* TODO: Test remove_buffer, add_buffer, write, prepend_buffer */

	if (start)
		tt_int_op(orig_length, ==, evbuffer_get_length(buf));

end:
	if (buf)
		evbuffer_free(buf);

	if (tmp_buf)
		evbuffer_free(tmp_buf);
}

static void
test_evbuffer_add_iovec(void * ptr)
{
	struct evbuffer * buf = NULL;
	struct evbuffer_iovec vec[4];
	const char * data[] = {
		"Guilt resembles a sword with two edges.",
		"On the one hand, it cuts for Justice, imposing practical morality upon those who fear it.",
		"Conscience does not always adhere to rational judgment.",
		"Guilt is always a self-imposed burden, but it is not always rightly imposed."
		/* -- R.A. Salvatore, _Sojurn_ */
	};
	size_t expected_length = 0;
	size_t returned_length = 0;
	int i;

	buf = evbuffer_new();

	assert(buf);

	for (i = 0; i < 4; i++) {
		vec[i].iov_len  = strlen(data[i]);
		vec[i].iov_base = (char*) data[i];
		expected_length += vec[i].iov_len;
	}

	returned_length = evbuffer_add_iovec(buf, vec, 4);

	tt_int_op(returned_length, ==, evbuffer_get_length(buf));
	tt_int_op(evbuffer_get_length(buf), ==, expected_length);

	for (i = 0; i < 4; i++) {
		char charbuf[1024];

		memset(charbuf, 0, 1024);
		evbuffer_remove(buf, charbuf, strlen(data[i]));
		assert(strcmp(charbuf, data[i]) == 0);
	}

	assert(evbuffer_get_length(buf) == 0);
end:
	if (buf) {
		evbuffer_free(buf);
	}
}

static void
test_evbuffer_copyout(void *dummy)
{
	const char string[] =
	    "Still they skirmish to and fro, men my messmates on the snow "
	    "When we headed off the aurochs turn for turn; "
	    "When the rich Allobrogenses never kept amanuenses, "
	    "And our only plots were piled in lakes at Berne.";
	/* -- Kipling, "In The Neolithic Age" */
	char tmp[1024];
	struct evbuffer_ptr ptr;
	struct evbuffer *buf;

	(void)dummy;

	buf = evbuffer_new();
	assert(buf);

	tt_int_op(strlen(string), ==, 206);

	/* Ensure separate chains */
	evbuffer_add_reference(buf, string, 80, no_cleanup, NULL);
	evbuffer_add_reference(buf, string+80, 80, no_cleanup, NULL);
	evbuffer_add(buf, string+160, strlen(string)-160);

	tt_int_op(206, ==, evbuffer_get_length(buf));

	/* First, let's test plain old copyout. */

	/* Copy a little from the beginning. */
	tt_int_op(10, ==, evbuffer_copyout(buf, tmp, 10));
	tt_int_op(0, ==, memcmp(tmp, "Still they", 10));

	/* Now copy more than a little from the beginning */
	memset(tmp, 0, sizeof(tmp));
	tt_int_op(100, ==, evbuffer_copyout(buf, tmp, 100));
	tt_int_op(0, ==, memcmp(tmp, string, 100));

	/* Copy too much; ensure truncation. */
	memset(tmp, 0, sizeof(tmp));
	tt_int_op(206, ==, evbuffer_copyout(buf, tmp, 230));
	tt_int_op(0, ==, memcmp(tmp, string, 206));

	/* That was supposed to be nondestructive, btw */
	tt_int_op(206, ==, evbuffer_get_length(buf));

	/* Now it's time to test copyout_from!  First, let's start in the
	 * first chain. */
	evbuffer_ptr_set(buf, &ptr, 15, EVBUFFER_PTR_SET);
	memset(tmp, 0, sizeof(tmp));
	tt_int_op(10, ==, evbuffer_copyout_from(buf, &ptr, tmp, 10));
	tt_int_op(0, ==, memcmp(tmp, "mish to an", 10));

	/* Right up to the end of the first chain */
	memset(tmp, 0, sizeof(tmp));
	tt_int_op(65, ==, evbuffer_copyout_from(buf, &ptr, tmp, 65));
	tt_int_op(0, ==, memcmp(tmp, string+15, 65));

	/* Span into the second chain */
	memset(tmp, 0, sizeof(tmp));
	tt_int_op(90, ==, evbuffer_copyout_from(buf, &ptr, tmp, 90));
	tt_int_op(0, ==, memcmp(tmp, string+15, 90));

	/* Span into the third chain */
	memset(tmp, 0, sizeof(tmp));
	tt_int_op(160, ==, evbuffer_copyout_from(buf, &ptr, tmp, 160));
	tt_int_op(0, ==, memcmp(tmp, string+15, 160));

	/* Overrun */
	memset(tmp, 0, sizeof(tmp));
	tt_int_op(206-15, ==, evbuffer_copyout_from(buf, &ptr, tmp, 999));
	tt_int_op(0, ==, memcmp(tmp, string+15, 206-15));

	/* That was supposed to be nondestructive, too */
	tt_int_op(206, ==, evbuffer_get_length(buf));

end:
	if (buf)
		evbuffer_free(buf);
}

void evbufTest()
{
	int getTimes = 1000;

	Tick tick("socket get address test");

	test_evbuffer();
	test_evbuffer_remove_buffer_with_empty();
	test_evbuffer_reserve2(NULL);
	test_evbuffer_reserve_many(NULL);
	test_evbuffer_expand(NULL);
	test_evbuffer_reference(NULL);
	test_evbuffer_readln(NULL);
	test_evbuffer_search_eol(NULL);
	// test_evbuffer_iterative(NULL);
	test_evbuffer_find(NULL);
	test_evbuffer_ptr_set(NULL);
	test_evbuffer_search(NULL);
	test_evbuffer_prepend(NULL);
	test_evbuffer_peek(NULL);
	test_evbuffer_peek_first_gt(NULL);
	test_evbuffer_freeze("start");
	test_evbuffer_add_iovec(NULL);
	test_evbuffer_copyout(NULL);

	double speed = tick.endTick() / getTimes;
	double per = 1.0f / speed;
	LOG_WARN << "avg cost = " << speed << ", per second = " << per;
}

void evbufSpeedTest()
{
	int times = 100000;

	{
		Tick tick("evbufSpeedTest");

		struct evbuffer *buf = evbuffer_new();
		const char *abc = "abcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyz";

		for (int i = 0; i < times; i++) {
			evbuffer_add(buf, abc, strlen(abc));
		}

		evbuffer_free(buf);
		double speed = tick.endTick() / times;
		double per = 1.0f / speed;
		LOG_WARN << "avg cost = " << speed << ", per second = " << per;
	}

	{
		Tick tick("bufferSpeedTest");

		Buffer buf;
		const char *abc = "abcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyzabcdefghijklmnopqrstvuwxyz";

		for (int i = 0; i < times; i++) {
			buf.append(abc, strlen(abc));
		}

		double speed = tick.endTick() / times;
		double per = 1.0f / speed;
		LOG_WARN << "avg cost = " << speed << ", per second = " << per;
	}
}


class BindTest
{
public:
	BindTest()
		: m_num(0)
	{
	}

public:
	void addnum(std::string *str)
	{
		m_num *= 2;
		m_num += 1;

		if (m_num > 10000000) {
			m_num = 1;
		}

		m_num++;
		m_num += str->size();
	}

	int m_num;
};


union epoll_data_fake {
	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
} ;

struct epoll_event_fake {
	uint32_t events;  /* Epoll events */
	epoll_data_fake data;    /* User data variable */
};

void test()
{
	int times = 100;
	{
		Tick tick("mutex performance test");

		int num = 0;
		mutex_t lock;
		for(int i = 0; i < times; i++) {
			lock.lock();
			num++;
			lock.unlock();
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	{
		Tick tick("atomic test");

		int num = 0;
		for(int i = 0; i < times; i++) {
			atomictool::inc(&num);
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	Buffer buf;
	char text[] = "hello word, test";
	buf.append(text, strlen(text));

	std::string str = text;

	times = 10000;
	{
		BindTest bt;
		Tick tick("task_t test");

		for(int i = 0; i < times; i++) {
			Task t = boost::bind(&BindTest::addnum, &bt, &str);
			t.run();
			t.release();
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	// 	{
	// 		Tick tick("bi::function test");
	//
	// 		for(int i = 0; i < times; i++) {
	// 			bi::function<void (std::string*)> func = bi::bind(&GateServer::addnum, this, _1);
	// 			func(&str);
	// 		}
	//
	// 		double speed = tick.endTick() / times;
	// 		double count = 1.0f / speed;
	// 		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	// 	}

	times = 10000;
	Buffer copyBuf;
	{
		ObjectPool<Buffer> m_bufferPool;
		m_bufferPool.init(5000, 5000);

		Tick tick("buffer pool test");

		for(int i = 0; i < times; i++) {
			Buffer *buf = m_bufferPool.alloc(1024);
			buf->append(text, strlen(text));
			m_bufferPool.free(buf);
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	{
		Tick tick("stack alloc test");

		for(int i = 0; i < times; i++) {
			Buffer *buf = new Buffer(1024);
			buf->append(text, strlen(text));
			delete buf;
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	{
		Tick tick("公共缓存");

		Buffer publicBuf(strlen(text) * times);

		for(int i = 0; i < times; i++) {
			publicBuf.append(text, strlen(text));
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	int variableTestNum = 10000;
	{
		Tick tick("global packet test");

		for(int i = 0; i < variableTestNum; i++) {
			LoginReq *req = msgtool::allocRecvPacket<LoginReq>();
			req->set_clientversion(100);
			req->set_deviceid("1273ab23c3390fe840a9e0");
			req->set_notifyid("notifyid-00134678");
			req->set_username("psy_micgit");
			req->set_zoneid(1);
			req->set_userid(100000001);
			req->set_playerid(14560034);
			req->set_job("sword");
			req->set_logintype(LoginReq_LoginType_NewRole);
			req->set_authtype(1);
			req->set_authkey("2ab456b6b2b1b6b1bb2b");

			msgtool::freePacket(req);
		}

		double speed = tick.endTick() / variableTestNum;
		double count = 1.0f / speed;
		LOG_WARN << "avg cost = " << speed << ", per second = " << count;
	}

	{
		Tick tick("stack allocate packet test");

		for(int i = 0; i < variableTestNum; i++) {
			LoginReq localreq;

			LoginReq *req = &localreq;
			req->set_clientversion(100);
			req->set_deviceid("1273ab23c3390fe840a9e0");
			req->set_notifyid("notifyid-00134678");
			req->set_username("psy_micgit");
			req->set_zoneid(1);
			req->set_userid(100000001);
			req->set_playerid(14560034);
			req->set_job("sword");
			req->set_logintype(LoginReq_LoginType_NewRole);
			req->set_authtype(1);
			req->set_authkey("2ab456b6b2b1b6b1bb2b");
		}

		double speed = tick.endTick() / variableTestNum;
		double count = 1.0f / speed;
		LOG_WARN << "avg cost = " << speed << ", per second = " << count;
	}

	{
		int getTimes = 1000;

		Tick tick("socket get address test");

// 		Listener *listener = Server::instance->m_lan.listen("127.0.0.1", 21005, *Server::instance);
//
// 		for(int i = 0; i < getTimes; i++) {
// 			NetAddress peeraddr(socktool::getLocalAddr(listener->socket()));
// 		}

		double speed = tick.endTick() / getTimes;
		double per = 1.0f / speed;
		LOG_WARN << "avg cost = " << speed << ", per second = " << per;
	}

	// evbufTest();

	evbufSpeedTest();
}