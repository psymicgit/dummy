T.namespace("AjaxDt");
T.AjaxDt = function(el, config) {
	this.el = T.get(el);
	this.config = config;
	T.AjaxDt.superclass.constructor.call(this);
	this.init();
}

T.extend(T.AjaxDt, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initEvt();
	},
	initDom :function() {
		this.dom = {};
	},
	initEvt :function() {
		this.el.bind("click", this.onClick.createDelegate(this));
		this.el.find("tr").hover(function(){
			if($(this).hasClass(".lazy_tr"))
			{
				$(this).addClass("lazy_tr_hover");
			}
			else
			{
				$(this).addClass("hover");
			}
		}, function(){
			if($(this).hasClass(".lazy_tr"))
			{
				$(this).removeClass("lazy_tr_hover");
			}
			else
			{
				$(this).removeClass("hover");
			}
		});
	},
	load :function(url) {
		if(!url)return;
		var self = this;
		
		$.ajax({
			url :url,
			success :function(text) {
				self.el.parent().empty().append(text);
			},
			error :function(request) {
				T.showAjaxReqError(request, "/");
			}
		});
	},
	onClick :function(e) {
		
		var t = $(e.target);
		if(t.is("a"))
		{
			if(t.parent().hasClass("pagination"))
			{
				e.stopPropagation();
				e.preventDefault();
				this.load(t.attr("href"));
				return false;
			}
			
			var type = t.attr("type");
			
			if(type == "layer") {
				e.stopPropagation();
				e.preventDefault();
				var width = t.attr("laywidth") || 950;
				var height = t.attr("layheight") || 300;
					width = parseInt(width);
					height = parseInt(height);
				p2cMessage.layerShow(t.attr("title"), t.attr("href"), width, height);
				return false;
			} 
			else if (type == "confirm")
			{
				e.stopPropagation();
				e.preventDefault();
				var title = t.attr("title") || "确定" + t.text(); 
				if(window.confirm(title)) {
					var href = t.attr("href");
						href = href.indexOf("?") != -1 ? (href + "&t_confirm=1") :(href + "?t_confirm=1"); 
					document.location.href=href;
				}
				return false;
			}
			else if(type == "tab") 
			{
				e.stopPropagation();
				e.preventDefault();
				var title = t.attr("tabtitle");
				
				p2fMessage.tabAdd(title, t.attr("href"));
				return false;
			}
		}
	}
});


T.namespace("Dt");

T.Dt = function(el, config) {
	this.el = T.get(el);
	this.config = config;
	T.Dt.superclass.constructor.call(this);
	this.addEvent({
		"lazyLoad" : true,
		"clickRow" : true,
		"clickLink": true,
		"changeCheck" : true
	});
	this.init();
}

T.extend(T.Dt, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initEvt();
		if(this.config.check)
		{
			this.layoutCheckAll();
		}
	},
	initDom :function() {
		this.dom = {};
		this.dom.body = this.el.find("tbody");
		
		if(this.config.sort)
		{
			this.dom.sort = this.el.find("th.sortable");
		}
		
		if(this.config.check)
		{
			this.dom.checkAll = this.el.find("thead .num img");
			this.dom.checkRow = this.dom.body.find(".num img");
		}
		
		if(this.config.lazy)
		{
			this.dom.lazy = this.dom.body.find(".lazymore img"); 
		}
		
		if(this.config.more)
		{
			this.dom.more = this.dom.body.find("div.more_op");
		}
		
		if(this.config.edit)
		{
			this.dom.edit = this.dom.body.find("td.edit");
		}
	},
	initEvt :function() {
		//视觉效果
		this.dom.body.find("tr").hover(function(){
			if($(this).hasClass(".lazy_tr"))
			{
				$(this).addClass("lazy_tr_hover");
			}
			else
			{
				$(this).addClass("hover");
			}
		}, function(){
			if($(this).hasClass(".lazy_tr"))
			{
				$(this).removeClass("lazy_tr_hover");
			}
			else
			{
				$(this).removeClass("hover");
			}
		}).click(this.onClickRow.createDelegate(this));
		
		if(this.config.check)
		{
			//功能处理
			this.dom.checkAll.bind("click", this.onCheckAll.createDelegate(this));
			this.dom.checkRow.bind("click", this.onCheckRow.createDelegate(this));
		}
		
		if(this.config.sort)
		{
			/*
			 * .hover(function(){
				var img = $(this).find("img");
				if(img.hasClass("sort_desc")) {
					img.data("back", "sort_desc");
					img.removeClass("sort_desc").addClass("sort_asc");
				} else if(img.hasClass("sort_asc")){
					img.data("back", "sort_asc");
					img.removeClass("sort_asc").addClass("sort_desc");
				} else {
					img.removeClass("sort_asc").addClass("sort_desc");
				}
			},function(){
				var img = $(this).find("img");
				if(!img.data("break")) {
					img.removeClass("sort_desc").removeClass("sort_asc");
					img.addClass(img.data("back"));
				}
				
			})
			 * */
			this.dom.sort.click(function(){
				var link = $(this).find("a");
				var href = link.attr("href");
				if(href) {
					$(this).find("img").data("break", true);
					window.location.href = href;
				}
			});
		}
		
		if(this.config.more)
		{
			this.dom.more.hover(function(){$(this).addClass("hover");}, function(){$(this).removeClass("hover");});
		}
		
		if(this.config.edit)
		{
			this.dom.edit.bind("click", this.onClickEditCol.createDelegate(this));
		}
	},
	layoutCheckAll :function() {
		var checked = this.dom.checkRow.filter(".checked");
		
		if(checked.length > 0 && checked.length == this.dom.checkRow.length) 
		{
			this.dom.checkAll.removeClass("unchecked").addClass("checked");
		} 
		else 
		{
			this.dom.checkAll.removeClass("checked").addClass("unchecked");
		}		
	},
	onClickRow :function(e) {
		var t = $(e.target);
		var tr = t.parents("tr:first");
		var td = t.is("td") ? t : t.parents("td:first");
		
		if(this.config.lazy) {
			this.onClickLazy(t, td, tr);
		}
		
		if(t.is("a")) {
			var type = t.attr("type");
			
			if(type == "layer") {
				e.stopPropagation();
				e.preventDefault();
				var width = t.attr("laywidth") || 950;
				var height = t.attr("layheight") || 300;
					width = parseInt(width);
					height = parseInt(height);
				p2cMessage.layerShow(t.attr("title"), t.attr("href"), width, height);
				return false;
			} 
			else if (type == "confirm")
			{
				e.stopPropagation();
				e.preventDefault();
				var title = t.attr("title") || "确定" + t.text(); 
				if(window.confirm(title)) {
					var href = t.attr("href");
						href = href.indexOf("?") != -1 ? (href + "&t_confirm=1") :(href + "?t_confirm=1"); 
					document.location.href=href;
				}
				return false;
			}
			else if(type == "tab") 
			{
				e.stopPropagation();
				e.preventDefault();
				var title = t.attr("tabtitle");
				
				p2fMessage.tabAdd(title, t.attr("href"));
				return false;
			}
			else {
				this.fireEvent("clickLink", [t, tr]);
			}
		} else {
			this.fireEvent("clickRow", [t, tr]);
		}
	},
	onClickLazy :function(t, td, tr) {
		var lazyUrl = tr.attr("lazy");
		
		if(!lazyUrl) {
			return;
		}
		
		if(tr.hasClass("loading_tr")) {
			return;
		}
		
		if(!tr.hasClass("minus_tr") && !tr.hasClass("plus_tr") && !tr.hasClass("load_tr")) 
		{
			tr.addClass("plus_tr");
		}
		
		if(tr.hasClass("load_tr"))
		{
			var unique = tr.attr("unique");
			var uniqueTr = tr.nextAll("tr[unique='"+ unique +"']");
			if(tr.hasClass("plus_tr"))
			{
				tr.removeClass("plus_tr").addClass("minus_tr");
				uniqueTr.removeClass("hide_tr");
			}
			else
			{
				tr.removeClass("minus_tr").addClass("plus_tr");
				uniqueTr.addClass("hide_tr");
			}
		} 
		else
		{
			this.lazyReq(lazyUrl, tr);
		}
		
	},
	lazyReq :function(lazyUrl, tr) {
		
		tr.removeClass("plus_tr").removeClass("minus_tr").addClass("loading_tr");
		
		var unique = T.getUnique();
		tr.attr("unique", unique);
		$.ajax({
			url :lazyUrl,
			type : "POST",
			dataType :"text",
			success :function(text) {
				tr.removeClass("loading_tr").addClass("load_tr").addClass("minus_tr");
				
				var segmentDom = $(text);
				var trSegmentDom = segmentDom.filter("tr");
				
				var rowIndex = tr.get(0).rowIndex + 1;
				var tableDom = tr.parents("table:first").get(0);
				
				trSegmentDom.each(function(r, trDom){
					var tableRow = tableDom.insertRow(rowIndex + r);
					
					if(trDom.className != "") {
						tableRow.className = trDom.className;
					}
					$(tableRow).attr("unique", unique).addClass("lazy_row");
					
					
					var tdSegmentDom = $(trDom).find("td");
					tdSegmentDom.each(function(c, tdDom){
						var tableCell = tableRow.insertCell(c);
							if(tdDom.className != "") {
								tableCell.className = tdDom.className;
							}
							$(tableCell).attr("colspan", $(tdDom).attr("colspan"));
							tableCell.innerHTML = $(tdDom).html();
					});
				});
				T.execScript(text);
			},
			error :function(request) {
				tr.removeClass("loading_tr").addClass("plus_tr");
				T.showAjaxReqError(request, "/");
			}
		});
	},
	onCheckAll :function(e) {
		var t = $(e.target);
		
		if(t.hasClass("checked")) 
		{
			this.changeCheck(this.dom.checkRow.filter(".checked"), "del", "all");
			t.removeClass("checked").addClass("unchecked");
			this.dom.checkRow.removeClass("checked").addClass("unchecked");
		} 
		else if(t.hasClass("unchecked")) 
		{
			this.changeCheck(this.dom.checkRow.filter(".unchecked"), "add", "all");
			t.removeClass("unchecked").addClass("checked");
			this.dom.checkRow.removeClass("unchecked").addClass("checked");
		}
		
	},
	onCheckRow :function(e) {
		e.stopPropagation();
		e.preventDefault();
		
		var t = $(e.target);
		if(t.hasClass("checked")) 
		{
			this.changeCheck(t, "del", "row");
			t.removeClass("checked").addClass("unchecked");
		} 
		else if(t.hasClass("unchecked")) 
		{
			this.changeCheck(t, "add", "row");
			t.removeClass("unchecked").addClass("checked");
		}
		
		this.layoutCheckAll();
	},
	changeCheck :function(input, operate, type) {
		if(this.config.check === true)
		{
			this.fireEvent("changeCheck", [input, operate, type]);
			return false;
		}
		
		//以下功能是用来 选择 回填到父frame 值中的功能
		var checkconf = this.config.check;
		
			
		var records = [];
		var values = [];
		input.each(function(k, item){
			
			var tr = $(item).parents("tr:first");
			
			var value = $(item).attr("data");
			var record = {};
				record[checkconf.value] = value;
			
			if(checkconf.name) {
				var name = tr.find("td").eq(checkconf.namecell).text();
				record[checkconf.name] = $.trim(name)+"_"+value;
			}
			
			records.push(record);
			values.push(value);
		});
		
		if(operate == "add") {
			c2pMessage.fireData(checkconf.fireAdd, records);
		} else if(operate == "del") {
			c2pMessage.fireData(checkconf.fireDel, values);
		}
		
		this.fireEvent("changeCheck", [input, operate, type]);
	},
	onClickEditCol :function(e) {
		var td = $(e.target);
		if(!td.is("td") && !td.is("input")) {
			td = td.parents("td:first");
		}
		var text = $.trim(td.text());
		var width = td.width();
		
		var input = $("<input type='text' />");
		var self = this;
			input.css({width: width-20});
			input.addClass("textbox");
			input.val(text);
			td.addClass("editing");
			td.empty().append(input);
			
			input.bind("focus", function(){
				$(this).data("old", this.value);
				T.setCaretPosition(this, this.value.length);
			});
			input.bind("blur", function(){
				var $this = $(this);
				var td = $this.parents("td:first");
					td.removeClass("editing").append($this.val());
					
				if($this.val() == $this.data("old"))
				{
					$this.remove();
				} 
				else 
				{
					var tr = td.parents("tr:first");
					if($this.val() == "") 
					{
						var ovalue = $this.data("old");
						td.empty().text(ovalue);
						return;
					}
					self.saveEdit(tr, td, $this);
				}
			});
			input.bind("keyup", function(e){
				if(e.keyCode == 13) {
					this.blur();
				}
			});
			input.focus();
	},
	saveEdit :function(tr, td, input) {
		
		var col = tr.find("td").index(td);
		var th = this.el.find("thead th").eq(col);
		
		var ovalue = input.data("old");
		var id = tr.attr("data");
		var data = {id: id};
			data[th.attr("param")] = input.val();
		
		$.ajax({
			url : th.attr("url"),
			data : data,
			dataTyep : "json",
			suceess :function(resp) {
				resp = resp || {respCode:1, respDesc:"数据更新失败."};
				if(resp.respCode != 0) {
					return td.empty().append(ovalue); 
				} else {
					alert(resp.respDesc);
				}
			},
			error :function(request) {
				td.empty().append(ovalue);
				T.showAjaxReqError(request, "/");
			}
		});
	},
	getCheckRows :function() {
		var checked = [];
		this.dom.checkRow.filter(".checked").each(function(k, item){
			checked.push($(item).attr("data"));
		});
		return checked;
	},
	getCheckName :function(cellIndex) {
		var names = [];
		this.dom.checkRow.filter(".checked").each(function(k, item){
			var tr = $(item).parents("tr:first");
			var name = tr.find("td").eq(cellIndex).text();
			names.push($.trim(name));
		});
		return names;
	},
	selected :function(data) {
		
		if(data instanceof Array)
		{
			for(var i=0; i<data.length; i++)
			{
				this.dom.checkRow.filter("img[data='"+data[i]+"']").removeClass("unchecked").addClass("checked");
			}
			
			var checked = this.dom.checkRow.filter(".checked");
			
			if(checked.length > 0 && checked.length == this.dom.checkRow.length) 
			{
				this.dom.checkAll.removeClass("unchecked").addClass("checked");
			} 
			else 
			{
				this.dom.checkAll.removeClass("checked").addClass("unchecked");
			}
		}
	}
});

T.namespace("TreeGrid");

T.TreeGrid = function(el, config) {
	this.el = T.get(el);
	this.config = config || {};
	T.TreeGrid.superclass.constructor.call(this);
	this.init();
};

T.extend(T.TreeGrid, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initEvt();
		this.initStructTree();
	},
	initDom :function() {
		this.dom = {};
		this.dom.body = this.el.find("tbody");
		if(this.config.edit)
		{
			this.dom.edit = this.dom.body.find("td.edit");
		}
	},
	initEvt :function() {
		this.dom.body.find("tr").hover(function(){
			$(this).addClass("hover");
		}, function(){
			$(this).removeClass("hover");
		}).click(this.onClickRow.createDelegate(this));
		
		if(this.config.edit)
		{
			this.dom.edit.bind("click", this.onClickCol.createDelegate(this));
		}
	},
	initStructTree :function() {
		var trs = this.el.find("tbody tr");
		var nodes = {};
			trs.each(function(k, item){
				var $item = $(item); 
				var id = $item.attr("data");
					nodes[id] = {id:id, pid:$item.attr("pid")};
			});
			
		for(var id in nodes) {
			
			var node = nodes[id];
			if(node.pid != "0")
			{
				var pid = node.pid;
				var parent = nodes[pid]; 
				if(parent) {
					parent.child = parent.child || []; 
					parent.child.push(node); 
				}
			}
		}
		
		this.nodes = nodes;
		this.trs = trs;
	},
	onClickCol :function(e) {
		var td = $(e.target);
		if(!td.is("td") && !td.is("input")) {
			td = td.parents("td:first");
		}
		var text = $.trim(td.text());
		var width = td.width();
		
		var input = $("<input type='text' />");
		var self = this;
			input.css({width: width-20});
			input.addClass("textbox");
			input.val(text);
			td.addClass("editing");
			td.empty().append(input);
			
			input.bind("focus", function(){
				$(this).data("old", this.value);
				T.setCaretPosition(this, this.value.length);
			});
			input.bind("blur", function(){
				var $this = $(this);
				var td = $this.parents("td:first");
					td.removeClass("editing").append($this.val());
					
				if($this.val() == $this.data("old"))
				{
					$this.remove();
				} 
				else 
				{
					var tr = td.parents("tr:first");
					if($this.val() == "") 
					{
						var ovalue = $this.data("old");
						td.empty().text(ovalue);
						return;
					}
					self.saveEdit(tr, td, $this);
				}
			});
			input.bind("keyup", function(e){
				if(e.keyCode == 13) {
					this.blur();
				}
			});
			input.focus();
	},
	saveEdit :function(tr, td, input) {
		
		var col = tr.find("td").index(td);
		var th = this.el.find("thead th").eq(col);
		
		var ovalue = input.data("old");
		var id = tr.attr("data");		
		var data = {id: id};
			data[th.attr("param")] = input.val();
		
		$.ajax({
			url : th.attr("url"),
			data : data,
			dataTyep : "json",
			suceess :function(resp) {
				resp = resp || {respCode:1, respDesc:"数据更新失败."};
				if(resp.respCode != 0) {
					return td.empty().append(ovalue); 
				} else {
					alert(resp.respDesc);
				}
			},
			error :function(request) {
				td.empty().append(ovalue);
				T.showAjaxReqError(request, "/");
			}
		});
	},
	onClickRow :function(e) {
		
		var t = $(e.target);
		var td = t.parents("td:first");
		
		if(td.hasClass("t_menu"))
		{
			if(t.is("input[type='checkbox']")) {
				this.changeCheck(td.parent(), !t.prop("checked"));
			} else {
				this.changeCheck(td.parent());
			}
			this.refreshCheck(td.parent());
		}
	},
	changeCheck :function(tr, checked) {
		
		var id = tr.attr("data"), node;
		if(!id || !(node = this.nodes[id])) 
		{
			return;
		}
		
		var input = tr.find("td.t_menu input");
		if(typeof checked == "undefined") 
		{
			checked = input.prop("checked");
		}
			checked ? input.prop("checked", false): input.prop("checked", true);
		
		if(node.child) {
			
			for(var i=0; i<node.child.length; i++)
			{
				var childNode = node.child[i];
				var childTr = this.trs.filter("[data='"+childNode.id+"']");
				this.changeCheck(childTr, checked);
			}
		}			
	},
	refreshCheck :function(tr) {
		
		var pid = tr.attr("pid"), node;
		if(!pid || !(node = this.nodes[pid])) 
		{
			return;
		}
		
		var checked = tr.find("td.t_menu input").prop("checked");
		var parentTr = this.trs.filter("[data='"+pid+"']");
		
		if(checked) {
			parentTr.find("td.t_menu input").prop("checked", checked);
			this.refreshCheck(parentTr);
		}
		else
		{
			var status = this.getChildStatus(parentTr);
			if(status == false)
			{
				parentTr.find("td.t_menu input").prop("checked", checked);
				this.refreshCheck(parentTr);
			}
		}
		
	},
	getChildStatus :function(tr)
	{
		var id = tr.attr("data"), node;
		if(!id || !(node = this.nodes[id])) 
		{
			return false;
		}
		
		if(node.child) {
			
			for(var i=0; i<node.child.length; i++)
			{
				var childNode = node.child[i];
				var childTr = this.trs.filter("[data='"+childNode.id+"']");
				if(childTr.find("input").prop("checked"))
				{
					return true;
				}
				if(this.getChildStatus(childTr))
				{
					return true;
				}
			}
		}
		return false;
	}
});

/*权限菜单表格*/
T.namespace("PremissGrid");

T.PremissGrid = function(el, config) {
	this.el = T.get(el);
	this.config = config || {};
	T.PremissGrid.superclass.constructor.call(this);
	this.init();
};

T.extend(T.PremissGrid, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initEvt();
		this.initStructTree();
	},
	initDom :function() {
		this.dom = {};
		this.dom.body = this.el.find("tbody");
	},
	initEvt :function() {
		this.dom.body.find("tr").hover(function(){
			$(this).addClass("hover");
		}, function(){
			$(this).removeClass("hover");
		}).click(this.onClickRow.createDelegate(this));
		
	},
	initStructTree :function() {
		var lis = this.el.find("tbody li");
		var nodes = {};
			lis.each(function(k, item){
				var $item = $(item); 
				var id = $item.attr("data");
					nodes[id] = {id:id, pid:$item.attr("pid")};
			});
			
		for(var id in nodes) {
			
			var node = nodes[id];
			if(node.pid != "0")
			{
				var pid = node.pid;
				var parent = nodes[pid]; 
				if(parent) {
					parent.child = parent.child || []; 
					parent.child.push(node); 
				}
			}
		}
		
		this.nodes = nodes;
		this.lis = lis;
	},
	onClickRow :function(e) {
		
		var t = $(e.target);
		var li = t.parents("li:first");
		
		if(li.length > 0)
		{
			if(t.is("input[type='checkbox']")) {
				this.changeCheck(li, !t.prop("checked"));
			} else {
				this.changeCheck(li);
			}
			this.refreshCheck(li);
		}
	},
	changeCheck :function(li, checked) {
		
		var id = li.attr("data"), node;
		if(!id || !(node = this.nodes[id])) 
		{
			return;
		}
		
		var input = li.find("input");
		if(typeof checked == "undefined") 
		{
			checked = input.prop("checked");
		}
			checked ? input.prop("checked", false): input.prop("checked", true);
		
		if(node.child) {
			
			for(var i=0; i<node.child.length; i++)
			{
				var childNode = node.child[i];
				var childLi = this.lis.filter("[data='"+childNode.id+"']");
				this.changeCheck(childLi, checked);
			}
		}			
	},
	refreshCheck :function(li) {
		
		var pid = li.attr("pid"), node;
		if(!pid || !(node = this.nodes[pid])) 
		{
			return;
		}
		
		var checked = li.find("input").prop("checked");
		var parentLi = this.lis.filter("[data='"+pid+"']");
		
		if(checked) {
			parentLi.find("input").prop("checked", checked);
			this.refreshCheck(parentLi);
		}
		else
		{
			var status = this.getChildStatus(parentLi);
			if(status == false)
			{
				parentLi.find("input").prop("checked", checked);
				this.refreshCheck(parentLi);
			}
		}
		
	},
	getChildStatus :function(li)
	{
		var id = li.attr("data"), node;
		if(!id || !(node = this.nodes[id])) 
		{
			return false;
		}
		
		if(node.child) {
			
			for(var i=0; i<node.child.length; i++)
			{
				var childNode = node.child[i];
				var childLi = this.lis.filter("[data='"+childNode.id+"']");
				if(childLi.find("input").prop("checked"))
				{
					return true;
				}
				if(this.getChildStatus(childLi))
				{
					return true;
				}
			}
		}
		return false;
	}
});

T.namespace("Tab");
T.Tab = function(el) {
	this.el = T.get(el);
	T.Tab.superclass.constructor.call(this);
	this.init();
};

T.extend(T.Tab, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initEvt();
	},
	initDom :function() {
		this.headli = this.el.find(".tabhead li");
		this.body01 = this.el.find(".tabbody .body01");
	},
	initEvt :function() {
		var self = this;
		this.headli.bind("click", function(e){
			var $this = $(this);
			var index = self.headli.index($this);
				
				self.headli.removeClass("current");
				$this.addClass("current");
				
				self.body01.removeClass("current");
				self.body01.eq(index).addClass("current");
			
			var t = $(e.target);
			if(t.is("a")) {
				e.stopPropagation();
				e.preventDefault();
			}
			
			self.loadBody($this, self.body01.eq(index));
			
			return false;
		});
		this.headli.hover(function(){
			$(this).addClass("hover");
		}, function(){
			$(this).removeClass("hover");
		});
		
		this.loadBody(this.headli.eq(0), this.body01.eq(0));
	},
	loadBody :function(headli, body01) {
		if(headli.attr("loaded") == 1) {
			return;
		}
		headli.attr("loaded", 1);
		
		var a = headli.find("a");
		if(a.attr("href").indexOf("javascript") != -1) {
			return;
		}
		body01.empty().append("<p class='loading'>数据加载中...</p>");
		$.ajax({
			url :a.attr("href"),
			type :"post",
			dataType :"text",
			success :function(text) {
				body01.empty().append(text);
			},
			error :function(request) {
				headli.attr("loaded", 0);
				T.showAjaxReqError(request);
			}
		});
	}
});

T.namespace("Cascade");
T.Cascade = function(config) {
	this.config = config;
	T.Cascade.superclass.constructor.call(this);
	this.init();
};

T.extend(T.Cascade, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initEvt();
		this.initReq();
	},
	initDom :function() {
		this.parent = $("#" + this.config.pid);
		this.child = $("#" + this.config.cid);
	},
	initEvt :function() {
		this.parent.bind("change", this.onParent.createDelegate(this));
	},
	initReq :function() {
		if(this.config.purl) 
		{
			this.loadData(this.parent, 0, this.config.pname, this.config.purl, this.config.pvalue);
		}
		
		if(this.config.curl) 
		{
			var pid = this.parent.val();			
			this.loadData(this.child, pid, this.config.cname, this.config.curl, this.config.cvalue);
		}
	},
	onParent :function() {
		var pid = this.parent.val();
		
		this.loadData(this.child, pid, this.config.cname, this.config.curl);
	},
	loadData :function(select, id, cname, curl, value) {
		var self = this;
		var data = {};
			data[cname] = id;

			select.empty().append("<option value=''>数据获取中</option>");
			
		var title = select.attr("title");
			title = title || "请选择";
		$.ajax({
			url :curl,
			data :data,
			type :"post",
			dataType :"json",
			success :function(resp) {
				if(resp && resp.resp_code == 0) 
				{
					cname = cname.replace("ta_", "");
					var o_cname = resp[cname];
					
					if(o_cname) {
						var options = ["<option value=''>"+title+"</option>"];
						for(var i=0; i < o_cname.length; i++)
						{
							options.push("<option "+(o_cname[i]['value'] == value ? "selected='selected'" : "")+" value='" + o_cname[i]['value'] + "'>" + o_cname[i]['name'] + "</option>");
						}
						select.empty().append(options.join(""));
					}
				} else {
						select.empty().append("<option value=''>"+title+"</option>");
				}
			},
			error :function(request) {
				select.empty().append("<option value=''>"+title+"</option>");
				T.showAjaxReqError(request);
			}
		});
	}
});

T.namespace("ChooseData");
T.ChooseData = function(el, config) {
	this.el = T.get(el);
	this.config = config;
	T.ChooseData.superclass.constructor.call(this);
	this.addEvent({
		"editRow" : true
	});
	this.init();
};

T.extend(T.ChooseData, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initEvt();
		this.renderData(this.getData());
	},
	initDom :function() {
		this.dom = {};
		this.dom.tbody = this.el.find("tbody");
		this.dom.thead = this.el.find("thead");
	},
	initEvt :function() {
		this.el.bind("click", this.onClick.createDelegate(this));
	},
	onClick :function(e) {

		var t = $(e.target);
		if(t.is("a")) {
			if(t.hasClass("del")) {
				this.delData(t.parents("tr:first").attr("key"));
			} else if(t.hasClass("edit")) {
				this.editData(t.parents("tr:first"));
			}
		}
		
	},
	getKey :function() {
		return this.config.names[0];		
	},
	getInputs :function() {
		var inputs = [];
		var names = this.config.names;
		var input = "";
			for(var i=0; i<names.length; i++) {
				input = this.el.find("input[name^='"+ names[i] +"']");
				if(input.length > 0) {
					inputs.push(input);
				}
			}
		return inputs;
	},
	addData :function(appends) {
		var records = this.getData();
		var key = this.getKey();
		
		if(records.length == 0) {
			this.dom.tbody.empty();
		}
		
		var length = records.length;
		
		if(appends.length)
		{
			for(var i=0; i<appends.length; i++)
			{
				this.dom.tbody.append(this.getRow(appends[i], key, length + 1));
				length++;
			}
		} 
		else
		{
			this.dom.tbody.append(this.getRow(appends, key, length + 1));
		}
	},
	delData :function(values) {
		
		var records = this.getData();
		var key = this.getKey();
		var newRecords = [];
		
		for(var i=0; i<records.length; i++)
		{
			var record = records[i];
			
			if(typeof values == "object")
			{
				var isexist = true;
				for(var j=0; j<values.length; j++)
				{
					if(record[key] == values[j])
					{
						isexist = false;
						break;
					}
				}
				if(isexist) 
				{
					newRecords.push(record);
				}
			}
			else 
			{
				if(record[key] != values)
				{
					newRecords.push(record);
				}
			}
		}
		this.renderData(newRecords);
	},
	editData :function(row) {
		var params = [];
		var names = this.config.names, value;
		for(var i=0; i<names.length; i++) {
			value = row.find("input[name^='"+names[i]+"']").val();
			value = encodeURIComponent(value);
			params.push(names[i] + "=" + value);
		}
		
		this.fireEvent("editRow", [params.join("&")]);
	},
	getData :function() {
		var inputs = this.getInputs();
		var input = null, name = "", data = {};
		
		if(inputs.length == 0) {
			return [];
		}
		
		for(var i=0; i<inputs.length; i++)
		{
			input = inputs[i];
			name = input.attr("name");
			name = name.replace("[]", "");
			if(name) {
				
				input.each(function(k, item){
					if(item.value != "") {
						if(!data[name]) {
							data[name] = [];
						}
						data[name].push(item.value);
					}
				});
			}
		}
		
		var records = [], record = null;
		
		var count = 0;
		for(var key in data)
		{
			var value = data[key];
			for(var j=0; j<value.length; j++)
			{
				if(count == 0) {
					record = {};
					record['index'] = j;
					record[key] = value[j];
					records.push(record);
				} else {
					for(var k=0; k<records.length; k++)
					{
						if(records[k].index == j) {
							record = records[k];
							record[key] = value[j];
							break;
						}
					}
				}
			}
			count++;
		}
		
		return records;
	},
	clear :function() {
		this.renderData([]);
	},
	renderData :function(records) {
		var trhtml = [], key = this.getKey();

		if(records.length > 0) {
			for(var i=0; i<records.length; i++) {
				var record = records[i];
				trhtml.push(this.getRow(record, key, i));
			}
		} else {
			trhtml.push("<tr><td colspan='" + (this.dom.thead.find("th").length) + "'>请选择</td></tr>");
		}
		
		this.el.find("input").remove();
		this.dom.tbody.empty().append(trhtml.join(""));
	},
	getRow :function(record, key, rowNum) {
		
		var trhtml =[], tdhtml = [];
		
		var names = this.config.names;
		for(var i=0; i<names.length; i++) {
			var name = names[i];
			if(typeof this.config.rewriteRow == 'function') {
				tdhtml.push(this.config.rewriteRow(name, record[name]));
			} else {
				tdhtml.push("<td>" + (record[name] == ""?"-":record[name]) + "<input type='hidden' name='"+name+"[]' value='" +record[name]+ "' />" + "</td>");
			}
		}
		if(this.config.editRow) {
			tdhtml.push("<td><a class='del' href='javascript:;'>删除</a><a style='margin-left:5px;' class='edit' href='javascript:;'>修改</a></td>");
		} else {
			tdhtml.push("<td><a class='del' href='javascript:;'>删除</a></td>");
		}
		
		trhtml.push("<tr", (rowNum%2!=0?" class='even' ":""), (" key='" + record[key] + "' "), ">", tdhtml.join(""), "</tr>");
		return trhtml.join("");
	}
});

T.namespace("ChooseInput");
T.ChooseInput = function(el, config) {
	this.el = T.get(el);
	this.config = config;
	T.ChooseInput.superclass.constructor.call(this);
};

T.extend(T.ChooseInput, T.Util.Observer, {
	getKey :function() {
		var key = this.el.attr("name");
			key = key.replace("[]", "");
		return key;
	},
	addData :function(records) {
		var data = this.getData();
		var key = this.getKey();
		for(var i=0; i<records.length; i++) {
			var isexist = false;
			
			for(var j=0; j<data.length; j++) {
				if(data[j] == records[i][key]) {
					isexist = true;
					break;
				}
			}
			if(!isexist) {
				data.push(records[i][key]);
			}
		}
		
		this.el.val(data.join(","));
	},
	delData :function(ids) {
		
		var data = this.getData();
		var newIds = [];
		for(var i=0; i<data.length; i++) {
			var isexist = true;
			for(var j=0; j<ids.length; j++) {
				
				if(data[i] == ids[j]) {
					isexist = false;
					break;
				}
			}
			if(isexist) {
				newIds.push(data[i]);
			}
		}
		
		this.el.val(newIds.join(","));
	},
	getData :function() {
		var data = this.el.val();
		if(data == "") {
			return [];
		} else {
			return data.split(",");
		}
	}
});


//-----------------------------------
//frameMessage  作用于通信框架工具类
//-----------------------------------
//page frame 和 child frame 之间的通信
var p2cMessage = {
	layerShow :function(title, url, width, height) {// 给父页面用
		var frameName = $(window.frameElement).attr("name");
		if(!p2cMessage.pageLayerInst) {
			p2cMessage.pageLayerInst = new FrameLayer();
		}
		p2cMessage.pageLayerInst.show({title : title, url: url, width: width, height: height});
	},
	refresh :function() {
		document.location.href = document.location.href;
	}
};

//child frame 与 page frame 之间的通信
var c2pMessage = {
	layerReset :function() {// 给子页面用
		if(window.parent && window.parent.p2cMessage && window.parent.p2cMessage.pageLayerInst) {
			window.parent.p2cMessage.pageLayerInst.reset(window.frameElement);
		}
	},
	layerHide :function() {
		if(window.parent && window.parent.p2cMessage && window.parent.p2cMessage.pageLayerInst) {
			window.parent.p2cMessage.pageLayerInst.hide(window.frameElement);
		}
	},
	
	showTip :function(text, type) {
		if(window.parent && window.parent.T) {
			return window.parent.T.getTip().show(text, type);
		}
	},
	
	getPagesData :function(input, singleInput) {// 给子页面用
		if(window.parent && window.parent.p2cMessage){
			var $j = window.parent.$;
			var inputs = $j("input[name='" + input + "']");
			
			if(singleInput) {
				var input_str = inputs.val();
				if(input_str) {
					return input_str.split(",");
				}
			} else {
				var input_arr = [];
				inputs.each(function(k, input){
					if(input.value != "") {
						input_arr.push(input.value);
					}
				});
			
				return input_arr;
			}			
		}
		return [];
	},
	
	fireData :function(name, param) {
		if(param == "on") {
			return function() {
				if(window.parent && window.parent.T && window.parent.T.Util){
					window.parent.T.Util.Bus.fire(name);
				}
			}
		} else {
			if(window.parent && window.parent.T && window.parent.T.Util){
				window.parent.T.Util.Bus.fire(name, param);
			}
		}
	},
	refresh :function() {
		if(window.parent && window.parent.p2cMessage) {
			window.parent.p2cMessage.refresh();
		}
	}
};

//page frame 与 最外的frame通信  
var p2fMessage = {
	
	leftMenuSelected : function(link) {
		if(window.parent && window.parent.pageCtrl) {
			window.parent.pageCtrl.menuSelected(link);
		}
	},
	layerShow :function(title, url, width, height) {
		if(window.parent && window.parent.frameMessage && window.parent.frameMessage.layerShow) {
			var frameName = $(window.frameElement).attr("name");
			window.parent.frameMessage.layerShow(title, url, width, height, frameName);
		}
	},
	
	confirmTip :function(title, tips, ok, cancel) {
		if(window.parent && window.parent.frameMessage && window.parent.frameMessage.layerShow) {
			window.parent.frameMessage.layerShow(title, function(){
				this.confirm(tips, ok, cancel);
			}, 460, null);
		}
	},
	
	tabReplace :function(name, link, menuLink, reload) {
		if(window.parent && window.parent.frameMessage && window.parent.frameMessage.tabReplace) {
			if(menuLink == "samelink")
			{
				menuLink = link;
			} 
			else if(menuLink == "noparam")
			{
				if(link.indexOf("?") != -1) {
					menuLink = link.substring(0, link.indexOf("?"));
				} else {
					menuLink = link;
				}
			}
			window.parent.frameMessage.tabReplace(name, link, menuLink, reload);
		}
	},
	tabAdd :function(name, url, id) {
		if(window.parent && window.parent.frameMessage && window.parent.frameMessage.tabAdd) {
			window.parent.frameMessage.tabAdd(name, url, id);
		}
	},
	tabRemove :function() {
		if(window.parent && window.parent.frameMessage && window.parent.frameMessage.tabRemove) {
			window.parent.frameMessage.tabRemove(window.frameElement);
		}
	},
	
	frameMaskHide :function() {
		if(window.parent && window.parent.frameMessage && window.parent.frameMessage.frameMaskHide) {
			window.parent.frameMessage.frameMaskHide();
		}
	},
	fixMobileFrameHeight :function() {
		if(T.mobile) {
            if(window.parent && window.parent.parent && window.parent.parent === window.parent.parent.parent)
            {
                if(window.parent.frameMessage)
                {
                    window.parent.frameMessage.fixMobileFrameHeight(window.frameElement);
                }
            }
        }
	}
};

var f2pMessage = {
	layerReset :function() {
		if(window.parent && window.parent.frameMessage && window.parent.frameMessage.layerReset) {
			window.parent.frameMessage.layerReset(window.frameElement);
		}
	}
}

//针对FrameMenu定位
$(function(){
	$("a").click(function(e){
		var $this = $(this);
		var locate = $this.attr("locate");
		
		if(locate == "yes") {
			if(window.parent == window) {
				return true;
			}
			var tablink = $this.attr("tablink") || $this.attr("link");
			var tabtitle = $this.attr("tabtitle") || $.trim($this.text());
			var reload = $this.attr("type") == "tab" ? false :true;
			p2fMessage.tabReplace(tabtitle, $this.attr("href"), tablink, reload);
			
			e.stopPropagation();
			e.preventDefault();
			return false;
		}
	});
	
	//隐藏Frame Mask
	p2fMessage.frameMaskHide();
	
	//修复手机浏览器高度
	p2fMessage.fixMobileFrameHeight();
});



//搜索条件处理
$.extend($.fn, {
    mymulti: function() {
    		$(this).find("ul.btn_box_op a").bind("click", function(e){
    			e.stopPropagation();
    			e.preventDefault();
    			var $this = $(this);
    			var input = $this.find("input");
    			if(input.length > 0) {
    				$this.removeClass("selected");
    				input.remove();
    			} else {
    				$this.addClass("selected");
    				var name = $this.parents("ul.btn_box_op:first").attr("name");
    				$this.append('<input type="hidden" value="' + $this.attr("data") + '" name="' + name + '" />');
    			}
    			return false;
    		}).each(function(){
    			var $this = $(this);
    			if($this.hasClass("selected")) {
    				var name = $this.parents("ul.btn_box_op:first").attr("name");
    				$this.append('<input type="hidden" value="' + $this.attr("data") + '" name="' + name + '" />');
    			}
    		});
    	
    	return this;
    }
});