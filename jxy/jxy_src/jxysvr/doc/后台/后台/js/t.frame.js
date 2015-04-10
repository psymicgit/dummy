/**
 * config = {eastId:"东部ID",westId:"西部ID",northId:"南部ID"};
 */
T.namespace("Layout");
T.Layout = function(el, config) {
	this.el = T.get(el);
	this.config = config;
	
	T.Layout.superclass.constructor.call(this);
	this.addEvent({
		"resize" : true
	});
	this.init();
}

T.extend(T.Layout, T.Util.Observer, {
	init :function() {
		this.initEvent();
	},
	initEvent :function() {
		$(window).bind("resize", this.layout.createDelegate(this));
	},
	layout :function() {
		if(T.mobile)
		{
			var east = $("#" + this.config.eastId);
			var west = $("#" + this.config.westId);
			var width = document.documentElement.clientWidth;
				west.css({width: 1920 - east.width() - T.getBox(west, "lr")});
				
			return;
		}
		var height = document.documentElement.clientHeight;
		var width = document.documentElement.clientWidth;
		var northHeight = $("#" + this.config.northId).outerHeight();
		
		var east = $("#" + this.config.eastId);
			east.css({height: height - northHeight - T.getBox(east, "tb")});
				
		var west = $("#" + this.config.westId);
			west.css({height: height - northHeight - T.getBox(west, "tb"), width: width - east.width() - T.getBox(west, "lr")});
		
		this.fireEvent("resize", [height, height - northHeight]);
	}
});


$.fn.extend({
	mousewheel:function(Func){
		return this.each(function(){
			var _self = this;
		    _self.D = 0;//滚动方向
			if($.browser.msie||$.browser.safari){
			   _self.onmousewheel=function(){_self.D = event.wheelDelta;event.returnValue = false;Func && Func.call(_self);};
			}else{
			   _self.addEventListener("DOMMouseScroll",function(e){
					_self.D = e.detail>0?-1:1;
					e.preventDefault();
					Func && Func.call(_self);
			   },false); 
			}
		});
	}
});

T.namespace("Scroll");
T.Scroll = function(el, config) {
	this.el = T.get(el);
	this.config = config || {};
	T.Scroll.superclass.constructor.call(this);
	this.init();
}
T.extend(T.Scroll, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initEvt();
		this.layout();
	},
	initDom :function() {
		this.dom = {};
		this.dom.scrollbarv = this.el.find(".scrollbarv");
		this.dom.scrollbarvbtn = this.dom.scrollbarv.find(".scrollbarvbtn");
		this.dom.scrollcen = this.el.find(".scrollcen");
	},
	initEvt :function() {
		$(window).resize(this.layout.createDelegate(this));
		var self = this;
		this.dom.scrollcen.mousewheel(function(){
			self.onMouseWheel(this.D);		
		});
	},
	layout :function(reset) {
		if(reset == "reset"){
			this.dom.scrollcen.css("top", 0);
			this.dom.scrollbarvbtn.css("top", 0);
		}
		this.layoutSC()
		this.layoutSBV();
	},
	layoutSC :function() {
		//修复底部问题
		var	cenTop = T.getStyleVal(this.dom.scrollcen, "top");
		var cenHeight = this.dom.scrollcen.height();
		var bot = cenHeight - this.el.height();
		
			if(bot > 0) {
				if(Math.abs(cenTop) > bot) {
					this.dom.scrollcen.css("top" , -bot);
				}
			} else {
				this.dom.scrollcen.css("top", 0);
			}
	},
	layoutSBV :function() {
		
		var cenHeight = this.dom.scrollcen.height();
		
		if(this.el.height() > cenHeight) {
			this.dom.scrollbarv.hide();
			return;
		} else {
			this.dom.scrollbarv.show();			
		}
		
		//设置按钮高度
		this.dom.scrollbarvbtn.css("height", (this.el.height() * this.el.height()) / cenHeight);
		
		var top = T.getStyleVal(this.dom.scrollcen, "top");
		
		//设置按钮的TOP值
		this.dom.scrollbarvbtn.css("top", (Math.abs(top) * this.el.height()) / cenHeight);
	},
	onMouseWheel :function(D) {
		
		var cenTop = T.getStyleVal(this.dom.scrollcen, "top");
		var setpNum = 108;
		
		if(D > 0)
		{
			cenTop = cenTop + setpNum;
			if(cenTop > 0) {
				cenTop = 0;
			}
			this.dom.scrollcen.css("top", cenTop);
		}
		else
		{
			var cenHeight = this.dom.scrollcen.height();
			var bot = cenHeight - this.el.height();
			if(bot < 0) {
				this.dom.scrollcen.css("top", 0);
				this.dom.scrollbarv.hide();
				return;
			}
			
			cenTop = cenTop - setpNum;
			
			if(Math.abs(cenTop) > bot) {
				cenTop = -bot;
			}
			
			this.dom.scrollcen.css("top", cenTop);
		}
		
		this.layoutSBV();
	}
});



/**
 * config = {id:"第一个节点，除根节点外", name:"如果有值，显示root节点", nodes:[], imgUrl:""};
 * */
T.namespace("Tree");
T.Tree = function(el, config) {
	this.el = T.get(el);
	this.config = config;
	T.Tree.superclass.constructor.call(this);
	this.init();
}

T.extend(T.Tree, T.Util.Observer, {
	init :function() {
		this.nodes = [];
		this.openedStore = this.config.storeFix + "_open";
		
		this.cache = new T.Util.Cache();
		
		this.addNode(this.config.nodes);
		
		this.initEvent();
	},
	initEvent :function() {
		this.el.bind("click", this.onClickTree.createDelegate(this));
	},
	onClickTree :function(e) {
		var t = $(e.target);
		if(t.is("a")) {
			var img = t.find("img");
			this.handlerImg(img, t.attr("data"));
			
		} else if(t.is("img")) {
			this.handlerImg(t, t.parent().attr("data"));
			
		} else if(t.is("span")) {
			var parent = t.parent();
			if(parent.is("a") && parent.attr("href") == "javascript:;") {
				var img = parent.find("img");
				this.handlerImg(img, parent.attr("data"));
				return;
			}
			if(this.selected) {
				this.selected.removeClass("selected");
			}
			this.selected = t.addClass("selected");
		}
	},
	handlerImg :function(img, data) {
		
		if(img.hasClass("tfolderopen")) {
			this.handlerOpenClose(data, 1);
			
		} else if(img.hasClass("tfolder")) {
			this.handlerOpenClose(data, 0);
			
		}
		
		if(!this.config.folder) {
			if(img.hasClass("tminus")) {
				this.handlerOpenClose(data, 1);
				
			} else if(img.hasClass("tminusbottom")) {
				this.handlerOpenClose(data, 1);
				
			} else if(img.hasClass("tplus")) {
				this.handlerOpenClose(data, 0);
				
			} else if(img.hasClass("tplusbottom")) {
				this.handlerOpenClose(data, 0);
				
			}
		}
		
	},
	handlerOpenClose :function(data, isOpen) {
		if(data != "") {
			var childcont = this.el.find("div[data='" + data + "']");
			var imgs = this.el.find("a[data='" + data + "'] img");
				folderImg = imgs.filter("img[class^=tfolder]");
				
			if(isOpen === 0) {
				if(this.openedArr.indexOf(data) < 0) {
					this.openedArr.push(data);
				}
				
				childcont.show();
				
				folderImg.removeClass("tfolder").addClass("tfolderopen");
				
				var tplus = imgs.filter(".tplus");
				
				if(tplus.length > 0) {
					tplus.removeClass("tplus").addClass("tminus");
				} else {
					imgs.filter(".tplusbottom").removeClass("tplusbottom").addClass("tminusbottom");
				}
				
			} else if(isOpen === 1) {
				this.openedArr.remove(data);
				
				childcont.hide();
				
				folderImg.removeClass("tfolderopen").addClass("tfolder");
				
				var tminus = imgs.filter(".tminus");
				
				if(tminus.length > 0) {
					tminus.removeClass("tminus").addClass("tplus");
				} else {
					imgs.filter(".tminusbottom").removeClass("tminusbottom").addClass("tplusbottom");
				}
			}
			
			this.cache.setItem(this.openedStore, this.openedArr.join(","));
		}
	},
	write :function(selectId) {
		this.html = [];
		
		var nodeData = this.getNodeById(this.config.id);
		var hasChildNode = this.hasChildNode(nodeData.id);
		var hasOpen = nodeData.isOpen;
		
		if(hasChildNode) {
			this.html.push('<a hidefocus="true" data="', nodeData.id, '" href="' , (nodeData.link?nodeData.link:"javascript:;") , '" ><img class="tfolder', (hasOpen?'open':''), '" src="', this.config.imgUrl, 's.gif" align="absbottom" /><span>' ,nodeData.name  , '</span></a><br />');
			
			this.recursedNodes = [];
			this.html.push('<div data="', nodeData.id, '"', (!hasOpen?' style="display: none;"':''), '>');
			this.writeNode(this.config.id, this.recursedNodes);
			this.html.push('</div>');
		} else {
			this.html.push('<a hidefocus="true" data="', nodeData.id, '" href="' , (nodeData.link?nodeData.link:"javascript:;") , '" ><img class="tbase" src="', this.config.imgUrl, 's.gif" align="absbottom" /><span>' , nodeData.name , '</span></a><br />');
		}
		
		this.el.append(this.html.join(""));
		
		if(selectId) {
			this.el.find("a[data='" + selectId + "'] span").addClass("selected");
		}
		
	},
	writeNode :function(parentId, recursedNodes) {
		
		for(var i=0; i<this.nodes.length; i++) {
			var nodeData = this.nodes[i];
			
			if(nodeData.parentId == parentId) {
				var lastSiblingNode = this.hasLastSibling(nodeData.id, nodeData.parentId);
				var hasChildNode = this.hasChildNode(nodeData.id);
				var hasOpen = nodeData.isOpen;
				
				// Write out line & empty icons
				for(var g = 0; g < recursedNodes.length; g++) {
					if(recursedNodes[g] == 1) {
						this.html.push('<img class="tline" src="', this.config.imgUrl, 's.gif" align="absbottom" />');
					} else {
						this.html.push('<img src="', this.config.imgUrl, 's.gif" align="absbottom" />');
					}
				}
				
				// put in array line & empty icons
				lastSiblingNode ? recursedNodes.push(0) : recursedNodes.push(1);
				
				// Write out join icons
				if (hasChildNode) {
					if (lastSiblingNode) {
						this.html.push('<a hidefocus="true" data="', nodeData.id, '" href="javascript:;"><img class="t', (hasOpen?'minus':'plus'), 'bottom" src="', this.config.imgUrl, 's.gif" align="absbottom" /></a>');
					} else {
						this.html.push('<a hidefocus="true" data="', nodeData.id, '" href="javascript:;"><img class="t', (hasOpen?'minus':'plus'), '" src="', this.config.imgUrl, 's.gif" align="absbottom" /></a>');
					}
				} else {
					if (lastSiblingNode) {
						this.html.push('<img class="tjoinbottom" src="', this.config.imgUrl, 's.gif" align="absbottom" />');
					} else {
						this.html.push('<img class="tjoin" src="', this.config.imgUrl, 's.gif" align="absbottom" />');
					}
				}
				
				// Start link
				this.html.push('<a hidefocus="true" data="', nodeData.id, '" href="' , (nodeData.link?nodeData.link:"javascript:;") , '" >');
				
				// Write out folder & page icons
				if (hasChildNode) {
					this.html.push('<img class="tfolder', (hasOpen?'open':''), '" src="', this.config.imgUrl, 's.gif" align="absbottom" />');
				} else {
					this.html.push('<img class="tpage" src="', this.config.imgUrl, 's.gif" align="absbottom" />');
				}
				
				// Write out node name
				this.html.push('<span>', nodeData.name, '</span>');

				// End link
				this.html.push('</a><br />');
				
				// If node has children write out divs and go deeper
				if (hasChildNode) {
					this.html.push('<div data="', nodeData.id, '"', (!hasOpen?' style="display: none;"':''), '>');
					this.writeNode(nodeData.id, recursedNodes);
					this.html.push('</div>');
				}
				
				// remove last line or empty icon 
				recursedNodes.pop();
			}
		}
	},
	hasLastSibling :function(nodeId, parentId) {
		var lastChild = 0;
		for (var i=0,length=this.nodes.length; i < length; i++) {
			var nodeData = this.nodes[i];
			if (nodeData.parentId == parentId) {
				lastChild = nodeData.id;
			}
		}
		if (lastChild == nodeId) return true;
		return false;
	},
	hasChildNode :function(parentId) {
		for (var i=0,length=this.nodes.length; i < length; i++) {
			var nodeData = this.nodes[i];
			if (nodeData.parentId == parentId) return true;
		}
		return false;
	},
	getNodeById :function(nodeId) {
		for (var i=0,length=this.nodes.length; i < length; i++) {
			var nodeData = this.nodes[i];
			if (nodeData.id == nodeId) return nodeData;
		}
		return;
	},
	addNode :function(nodes) {
		var openedStr = this.cache.getItem(this.openedStore);
		this.openedArr = (openedStr ? openedStr.split(",") : []);
		
		for(var i=0,length=nodes.length; i < length; i++) {
			var nodeData = nodes[i];
			
			if((typeof nodeData.isOpen == "undefined") && this.openedArr.indexOf(nodeData.id) >= 0) {
				nodeData.isOpen = true;
			}
			this.nodes.push(nodeData);
		}
	}
});


T.namespace("FrameTab");
T.FrameTab = function(el, config) {
	this.el = T.get(el);
	this.config = config;
	T.FrameTab.superclass.constructor.call(this);
	this.init();
};
T.extend(T.FrameTab, T.Util.Observer, {
	init :function() {
		this.initDom();
		this.initVal();
		this.initEvt();
		this.layout();
		
		if(this.config.firstName)
		{
			this.headwapul.find("a:first").text(this.config.firstName);
		}
		if(this.config.firstUrl)
		{
			this.body.find("iframe:first").attr("src", this.config.firstUrl);
			T.Util.Bus.fire("tabLoading", this.config.firstUrl);
		}
	},
	initDom :function() {
		this.head = this.el.find("div.tabhead");
		this.body = this.el.find("div.tabbody");
		this.headwap = this.head.find("div.tabwap");
		this.headwapul = this.headwap.find("ul"); 
		
		this.ltBtn = this.head.find("div.leftbtn");
		this.rtBtn = this.head.find("div.rightbtn");
		this.addBtn01 = this.head.find("li.addbtn01");
		this.addBtn02 = this.head.find("div.addbtn02");

		this.closeBtn = this.head.find("li.close em");
	},
	initEvt :function() {
		this.ltBtn.hover(function(){$(this).addClass("lthover");}, function(){$(this).removeClass("lthover");});
		this.rtBtn.hover(function(){$(this).addClass("rthover");}, function(){$(this).removeClass("rthover");});
		
		this.ltBtn.click(this.onClickLt.createDelegate(this));
		this.rtBtn.click(this.onClickRt.createDelegate(this));
		
		this.addBtn01.click(this.createTab.createDelegate(this));
		this.addBtn02.click(this.createTab.createDelegate(this));
		
		this.closeBtn.hover(function(){$(this).addClass("hover");}, function(){$(this).removeClass("hover");});
		this.closeBtn.click(this.onClickClose.createDelegate(this));
		
		this.head.find("li").not(".addbtn01").click(this.onClickTab.createDelegate(this));
		
		var self = this;
		$(window).resize(function(){
			self.layout();
			self.resetTab("reset");
		});
		
		if($.browser)
		{
			this.head.get(0).onselectstart=function(){return(false)}
		}
	},
	initVal :function() {
		var mr = this.head.find("li").eq(0).css("marginRight");
		this.mr = this.getInt(mr);
	},
	
	getInt :function(strNum) {
		strNum = strNum.replace("px", "");
		return parseInt(strNum);
	},
	layout :function() {
		var tabmr = this.headwap.css("marginRight");
			tabmr = this.getInt(tabmr);
		var tabml = this.headwap.css("marginLeft");
			tabml = this.getInt(tabml);
			
		this.headwap.css({width:this.el.width() - tabmr - tabml});
	},
	onClickLt :function() {
		var headwapWidth = this.headwap.width();
		var tabs = this.head.find("li").not('.addbtn01');
		
		var tabsWidth = 0;
		var scrollLeft = 0 - T.getStyleVal(this.headwapul, "marginLeft");
		
		for(var i=0; i<=tabs.length; i++) {
			tabsWidth += (tabs.eq(i).outerWidth() + this.mr);
			if(tabsWidth >= scrollLeft) {
				this.scrollLt(tabs.eq(i));
				break;
			}
		}
	},
	scrollLt :function(tab) {
		var headwapWidth = this.headwap.width();
		var tabs = this.head.find("li").not('.addbtn01');
		
		var tabsWidth = 0;
		
		for(var i=0; i<=tabs.length; i++) {
			if(tabs.get(i) == tab.get(0)) {
				
				//this.headwap.animate({scrollLeft: tabsWidth});
				this.headwapul.animate({"marginLeft": 0 - tabsWidth});
				break;
			}
			
			tabsWidth += (tabs.eq(i).outerWidth() + this.mr);
		}
		
		if(tab.get(0) == tabs.first().get(0)) {
			this.ltBtn.addClass("dislt");
		} else {
			this.ltBtn.removeClass("dislt");
		}
		
		this.rtBtn.removeClass("disrt");
	},
	onClickRt :function() {
		var headwapWidth = this.headwap.width();
		var tabs = this.head.find("li").not(".addbtn01");
		
		var tabsWidth = 0;
		var scrollLeft = 0 - T.getStyleVal(this.headwapul, "marginLeft");
		
		for(var i=0; i<=tabs.length; i++) {
			tabsWidth += (tabs.eq(i).outerWidth() + this.mr);
			if(tabsWidth >= scrollLeft) {
				this.scrollRt(scrollLeft);
				break;
			}
		}
	},
	scrollRt :function(scrollWidth) {
		var headwapWidth = this.headwap.width() + scrollWidth;
		var tabs = this.head.find("li").not(".addbtn01");
		var tab = null;
		var tabsWidth = 0;
		
		for(var i=0; i<tabs.length; i++) {
			if(tabsWidth > headwapWidth) {
				tab = tabs.eq(i);
				//this.headwap.animate({scrollLeft:scrollWidth + (tabsWidth - headwapWidth)});
				this.headwapul.animate({"marginLeft": 0 - (scrollWidth + (tabsWidth - headwapWidth))});
				break;
			} else {
				tabsWidth += (tabs.eq(i).outerWidth() + this.mr);
				if(tabs.length-1 == i) {
					tab = tabs.eq(i);
					//this.headwap.animate({scrollLeft:scrollWidth + (tabsWidth - headwapWidth)});
					this.headwapul.animate({"marginLeft": 0 - (scrollWidth + (tabsWidth - headwapWidth))});
				}
			}
		}
		
		if(tab && (tab.get(0) == tabs.last().get(0))) {
			this.rtBtn.addClass("disrt");
		} else {
			this.rtBtn.removeClass("disrt");
		}
		this.ltBtn.removeClass("dislt");
	},
	onClickClose :function(e) {
		var tab = $(e.target).parent();
		if(tab.is("li")) {
			this.removeByTab(tab);
		}
	},
	removeByTab :function(tab) {
		var tabs = this.head.find("li");
		var frames = this.body.find("iframe");
		
		var index = tabs.index(tab);
		
		var scrollLeft = (0 - T.getStyleVal(this.headwapul, "marginLeft")) - tab.outerWidth() - this.mr;
		
			if(tab.hasClass("current")) {
				this.showTab(tab.prev());		
			}
			tab.remove();
			frames.eq(index).remove();
			
		this.resetTab("remove", scrollLeft);
	},
	removeByFrame :function(frame) {
		var tabs = this.head.find("li");
		var frames = this.body.find("iframe");
		var index = frames.index(frame);
		var tab = tabs.eq(index);
		
		var scrollLeft = (0 - T.getStyleVal(this.headwapul, "marginLeft")) - tab.outerWidth() - this.mr;
		
			if(tab.hasClass("current")) {
				this.showTab(tab.prev());		
			}
			tab.remove();
			frames.eq(index).remove();	
	},
	onClickTab :function(e) {
		
		var t = $(e.target);
		
		if(t.is("li")) {
			t = t;
		} else if(t.is("a")) {
			t = t.parent();
		} else {
			return;
		}
		
		this.showTab(t);
		T.Util.Bus.fire("tabToggle", t.find("a"));
	},
	showTab :function(tab, name, link, menuLink, reload) {
		var tabs = this.head.find("li").not(".addbtn01");
		var frames = this.body.find("iframe");
		
		var index = tabs.index(tab);
			tabs.removeClass("current");
			frames.css("display", "none");
			
			tab.addClass("current");
			frames.eq(index).css("display", "block");
			
			if(typeof name === "string")
			{
				tab.find("a").text(name);
			}
			
			if(menuLink) 
			{
				tab.find("a").attr("link", menuLink);
			}
			
			if(typeof link === "string" && reload !== false) 
			{
				T.Util.Bus.fire("tabLoading", link);
				frames.eq(index).attr("src", link);				
			}
	},
	replaceTab :function(name, link, menuLink, reload) {
		
		var tabs = this.head.find("li").not(".addbtn01");
		var frames = this.body.find("iframe");
		var exist = false;
		
		for(var i=0; i<frames.length; i++)
		{
			var src = frames.eq(i).attr("src");
			if(src && link && src.indexOf(link) != -1) {
				var index = frames.index(frames.eq(i));
				var tab = tabs.eq(index);
				this.showTab(tab, name, link, menuLink, reload);
				return;
			}
		}
				
		var tab = tabs.filter(".current");
		if(tabs.index(tab) == 0 && link && link.indexOf(this.config.firstUrl) == -1) {
			if(tabs.length > 1) {
				this.showTab(tabs.eq(1), name, link, menuLink);
			} else {
				this.createTab(name, link, menuLink);
			}
		} else {
			this.showTab(tab, name, link, menuLink);
		}
	},
	createTab :function(name, link, menuLink) {
		var tabs = this.head.find("li").not(".addbtn01");
		var frames = this.body.find("iframe");
		var firstFrame = frames.eq(0);
		var maxTab = this.config.maxTab || 10;
		var link = link || this.config.blankUrl;
			
			//tab 存在就不创建
			if(typeof link !== "undefined" && link != this.config.blankUrl) 
			{
				for(var i=0; i<frames.length; i++)
				{
					var frame = frames.eq(i);
					var src = frame.attr("src");
					if(src.indexOf(link) != -1) 
					{
						var index = frames.index(frames.eq(i));
						var tab = tabs.eq(index);
						this.showTab(tab, name, link, menuLink);
						return false;
					}
				}
			}
			
			
			if(tabs.length > maxTab) {
				return alert("对不起，您打开的标签页超过 " + maxTab + " 大于，影响性能，建议您关闭一些无用标签页");	
			}
		var newTab = $('<li data="' + (typeof id !== "undefined"? id:'') + '" class="close"><a hidefocus="true" href="javascript:;">' + (typeof name === "string" ? name : "新建标签页") + '</a><em>x</em></li>');
			newTab.insertAfter(this.addBtn01.prev());
			
			newTab.click(this.onClickTab.createDelegate(this));
			newTab.find("em").hover(function(){$(this).addClass("hover")}, function(){$(this).removeClass("hover")}).click(this.onClickClose.createDelegate(this));
			newTab.find("a").attr("link", menuLink);
			
		var newFrame = $('<iframe data="' + (typeof id !== "undefined"? id:'') + '" class="frame" src="' + ($.browser.msie ? "#":link) + '" frameborder="0" scrolling="auto" width="100%;"></iframe>');
			newFrame.css({height: firstFrame.css("height")});
			this.body.append(newFrame);
			if($.browser.msie) {
				newFrame.attr("src", link);
			}
			this.showTab(newTab);
			this.resetTab("add");
	},
	getTabsWidth :function() {
		
		var tabs = this.head.find("li");
		
		var tabsWidth = 0;
		var mr = this.mr;
		tabs.each(function(k, item){
			tabsWidth += ($(item).outerWidth() + mr);
		});
		return tabsWidth;
	},
	resetTab :function(op, scrollLeft) {
		var headwapWidth = this.headwap.width();
		var tabsWidth = this.getTabsWidth();
		
		var addbtn01Width = this.head.find("div.addbtn01").outerWidth() + this.mr;
		
		if(headwapWidth <= (tabsWidth + addbtn01Width)) {
			this.head.addClass("showbtn");
			this.layout();
			headwapWidth = this.headwap.width();
			
			if(op == "add") {
				//this.headwap.animate({scrollLeft: tabsWidth - headwapWidth - 40});
				this.headwapul.animate({"marginLeft": 0 - (tabsWidth - headwapWidth - 40)});
			} else if(op == "remove") {
				//this.headwap.animate({scrollLeft: scrollLeft});
				if(scrollLeft < 0) {
					scrollLeft = 0;
				}
				this.headwapul.animate({"marginLeft": 0 - scrollLeft});
			} else if(op == "reset") {
				//this.headwap.animate({scrollLeft :0});
				this.headwapul.animate({"marginLeft": 0});
			}
		} else {
			this.head.removeClass("showbtn");
			this.layout();
			//this.headwap.animate({scrollLeft:0});
			this.headwapul.animate({"marginLeft": 0});
		}
	}
	
});


var frameMenu = function(el, nodes) {
	
	var el = T.get(el);
	
	/*
	conf = {
		nodes :config.nodes
	};
	*/
	
	var op = {
		init :function() {
			op.initDom();
			op.initMenu();
			op.initEvt();
			op.initScroll();
			
			return op;
		},
		initDom :function() {
			op.dom = {};
			op.dom.head = el.find(".left_tab_head");
			op.dom.body = el.find(".left_tab_body");
		},
		initEvt :function() {
			
			op.dom.headli = op.dom.head.find("li");
			op.dom.bodydd = op.dom.body.find("dd");
			op.dom.bodydv = op.dom.body.find(".tab_body");
			op.dom.bodydt = op.dom.body.find("dt");
			
			op.dom.headli.bind("click", op.onClickHead);
			op.dom.bodydd.bind("click", op.onClickBody);
			op.dom.bodydt.bind("click", function(){
				var _this = $(this);
				if(_this.hasClass("open")) {
					_this.removeClass("open").addClass("close");
					_this.nextAll("dd").css("display", "none");
				} else {
					_this.removeClass("close").addClass("open");
					_this.nextAll("dd").css("display", "");
				}
			});
			
			op.dom.headli.hover(function(){
				$(this).addClass("hover");
			}, function(){
				$(this).removeClass("hover");
			});
			
			op.dom.bodydd.hover(function(){
				$(this).addClass("hover");
			}, function(){
				$(this).removeClass("hover");
			});
			
			if(!op.dom.head.hasClass("developer"))
			{
				op.dom.bodydv.each(function(k, item) {
					var firstLink = $(item).find("dd:first a");
					op.dom.headli.eq(k).find("a").attr("href", firstLink.attr("href"));
				});
			}
		},
		onClickHead :function(e) {
			e.stopPropagation();
			e.preventDefault();
			
			op.dom.headli.removeClass("current");
			$(this).addClass("current");
			
			var index = op.dom.headli.index(this);
			op.dom.bodydv.removeClass("current");
			op.dom.bodydv.eq(index).addClass("current");
			
			if(op.scrollInst){
				op.scrollInst.layout("reset");
			}
			
			if(!op.dom.head.hasClass("developer"))
			{
				var alink = op.dom.bodydv.eq(index).find("dd:first a");
				T.Util.Bus.fire('tabReplace', alink);
			}
			
			return false;
		},
		onClickBody :function(e) {
			e.stopPropagation();
			e.preventDefault();
			
			op.dom.bodydd.removeClass("actived");
			$(this).addClass("actived");
			
			var alink = $(this).find("a");
			T.Util.Bus.fire('tabReplace', alink);
			
			return  false;
		},
		initScroll :function() {
			if(!T.mobile) {
				op.scrollInst = new T.Scroll(op.dom.body);
			}
		},
		initMenu :function() {
			
			var headhtml = ['<ul>'];
			var bodyhtml = [];
			for(var i=0; i<nodes.length; i++)
			{
				headhtml.push('<li'+(i==0?' class="current" ':'')+'><a  onclick="return false;" href="javascript:;">'+nodes[i].name+'</a></li>');
				bodyhtml.push('<div class="tab_body'+(i==0?' current ':'')+'">');

				var children01 = nodes[i].children;
				if(children01) {
						
					for(var j=0; j < children01.length; j++) {
						
						var children02 = children01[j].children;
						var isOpen = children01[j].open === false ? false: true;
						
						bodyhtml.push('<dl><dt class="'+(isOpen?'open':'close')+'">'+children01[j].name+'</dt>');
						
						for(var k=0; k < children02.length; k++) {
							//'+(i==0 && j==0 && k==0?' class="actived" ': '')+'
							bodyhtml.push('<dd', (isOpen?'':' style="display:none"'), '><a onclick="return false;" href="'+children02[k].link+'">'+children02[k].name+'</a></dd>');
						}
						
						bodyhtml.push('</dl>');								
					}
				}
				
				bodyhtml.push('</div>');						
			}
			headhtml.push('</ul>');
			
			op.dom.head.append(headhtml.join(""));
			op.dom.body.find(".scrollcen").append(bodyhtml.join(""));
		},
		select :function(link) {
			
			if(link) 
			{
					op.dom.body.find("a").each(function(k, alink){
						alink = $(alink);
						var href = alink.attr("href");
						if(href.indexOf(link) != -1) 
						{
							op.dom.headli.removeClass("current");
							op.dom.bodydv.removeClass("current");
							op.dom.bodydd.removeClass("actived");
							
							alink.parent().addClass("actived");
							
							var body = alink.parents(".tab_body:first");
								body.addClass("current");
								
							var index = op.dom.bodydv.index(body);
								op.dom.headli.eq(index).addClass("current");
							
							return false;
						}						
					});
			}
		}
	};
	
	return op;	
}



//-------------------------
//作用于iframe和外部框架通信工具
//-------------------------
var frameMessage = {
		
	/*frame 和 page 页面之间的通信*/
	frameTabInst :null,
	frameMenuInst :null,
	_init :function(nodes, baseUrl, blankUrl, permission) {

		T.Util.Bus.add('tabLoading', function(){
			frameMessage.frameMaskShow();
		});
		
		T.Util.Bus.add('tabReplace', function(alink){
			var link = alink.attr("href");
			frameMessage.tabReplace(alink.text(), link, link);
		});
		/*
		T.Util.Bus.add('tabToggle', function(alink){
			frameMessage.frameMenuInst.select(alink.attr("link"));
		});
		*/
		frameMessage._initFrameMenu(nodes);
		frameMessage._initFrameTab(baseUrl, blankUrl, permission);
	},
	_initFrameMenu :function(nodes) {
		if(!frameMessage.frameMenuInst) {
			frameMessage.frameMenuInst = frameMenu("j_left_tab", nodes).init();
		}
	},
	_initFrameTab :function(baseUrl, blankUrl, permission) {
		if(!frameMessage.frameTabInst) {
			//初始化第一个页面连接
			var firstA = $("#j_left_tab_body").find("dd a:first");
				firstA.parents("dd:first").addClass("actived");
			var firstLink = firstA.attr("href");
			var firstName =  $.trim(firstA.text());
			if(permission) {
				firstLink = baseUrl + "analysis/platform/summary.do";
				firstName =  "聚合首页";
			}
			frameMessage.frameTabInst = new T.FrameTab("j_main_tab", {firstName: firstName, firstUrl: firstLink, blankUrl:blankUrl});
		}
	},
	tabReplace :function(name, link, menuLink, reload) {
		if(frameMessage.frameTabInst) {
			frameMessage.frameTabInst.replaceTab(name, link, reload);
		}
		frameMessage.menuSelect(menuLink);
	},
	tabAdd :function(name, link, menuLink) {
		if(frameMessage.frameTabInst) {
			frameMessage.frameTabInst.createTab(name, link, menuLink);
		}
		frameMessage.menuSelect(menuLink);
	},
	tabRemove :function(frame) {
		if(frameMessage.frameTabInst) {
			frameMessage.frameTabInst.removeByFrame(frame);
		}
	},
	menuSelect :function(menuLink)
	{
		if(frameMessage.frameMenuInst)
		{
			frameMessage.frameMenuInst.select(menuLink);
		}
	},
	/*frame 和 page 页面之间的通信*/
	
	frameLayerInst :null,
	layerReset :function(frame) {
		if(frameMessage.frameLayerInst) {
			frameMessage.frameLayerInst.reset(frame);
		}
	},
	layerShow :function(title, url, width, height) {
		if(!frameMessage.frameLayerInst) {
			frameMessage.frameLayerInst = new FrameLayer();
		}
		frameMessage.frameLayerInst.show({title : title, url: url, width: width, height: height});
	},
	
	frameMaskInst :null,
	frameMaskShow :function() {
		if(!frameMessage.frameMaskInst) {
			frameMessage.frameMaskInst = new T.LoadTip("j_tab_body");
		}
		frameMessage.frameMaskInst.show("数据加载中", "loading");
	},
	frameMaskHide :function() {
		if(frameMessage.frameMaskInst)
		{
			frameMessage.frameMaskInst.hide();
		}
	},
	fixMobileFrameHeight :function(frame) {
		if(frame.contentDocument){
            var height = frame.contentDocument.body.offsetHeight;
            if(height > 0) {
            	$(frame).css("height", height);
            }
	    }else if(frame.Document && frame.Document.body.scrollHeight){
	        $(frame).css({height :frame.Document.body.scrollHeight + 100});
	    }
	}
}