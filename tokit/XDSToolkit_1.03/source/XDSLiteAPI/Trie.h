/*
**  Trie store/search functions (implemented using bit-skips)
**
**  By Mark T. Price
**  June 7, 1996
**
**  Converted to C++ class October 1, 1996
**  Converted to Abstract base class January 24, 2000
**  Added iterator class March 19, 2000
**
**  Derived class must define GetKey() to return the indexing key for the pointed
**  to data type.
*/
// ----------------------------------------------------------------------------
// 
// This file is part of the XDS Toolkit, Copyright (C) 2003-2004 Mark T. Price
// For conditions of distribution and use, see XdsLicense.h
//
// ----------------------------------------------------------------------------


#ifndef TRIE_INCL
#define TRIE_INCL

//#define TRIE_SUPPORT_PRINTING	// for debugging use only -- includes entry point to print contents of Trie

// forward declare family classes
class trie_node;
class CTrieIterator;

class CTrie
{
friend CTrieIterator;
private:
	virtual void GetKey(void *data, unsigned char **key, int *keylen) = 0;
	CTrie::CTrie(CTrie &trie2);		// prevent copying (never instantiated)

#ifdef TRIE_SUPPORT_PRINTING
	virtual void PrintKey(const char *szPrefix, void *data) = 0;
	void PrintNode(const char *szPrefix, char chHeader, trie_node *pNode);
#endif

protected:
	trie_node *m_root;

public:
	CTrie();
	virtual CTrie::~CTrie();

	void *Find(unsigned char *key, int keylen);
	void *Add(void *data);
//	void *Add(unsigned char *key, int keylen, void *data);
	void *Del(void *data);
	void *Del(unsigned char *key, int keylen);

#ifdef TRIE_SUPPORT_PRINTING
	void Print(void);
#endif
};

class CTrieIterator
{
private:
	CTrie     *m_trie;
	trie_node *m_node;

public:
	CTrieIterator(CTrie &trie);
	~CTrieIterator(void) { };

	bool operator --(void);		// returns false if this is the first entry in the trie
	bool operator ++(void);		// returns false if this is the last entry in the trie
	void *GetData(void) const;	// returns data element at current position
	bool AtEnd(void) const;		// returns true if iterator is at the end of the trie
	bool AtBegin(void) const;   // returns true if iterator is at the start of the trie
	void Rewind(void);
};

#endif // TRIE_INCL
