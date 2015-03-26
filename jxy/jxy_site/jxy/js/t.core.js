/**
 * date 2012-03-06
 * descript 此js文件为公共js文件包括以下组件, 依赖与jquery库
 	1、html5的支持
 	2、跨域的问题domain设置
 	3、ie6 背景图片修复
	4、Ajax不做缓存
 	5、表单校验
 	6、简单事件处理bus
*/

(function(){
	//-----------------------------
	//1、html5的支持
	//-----------------------------
	if(navigator.userAgent.indexOf("MSIE") != -1) {
	// iepp v2.1pre @jon_neal & @aFarkas github.com/aFarkas/iepp
	// html5shiv @rem remysharp.com/html5-enabling-script
	// Dual licensed under the MIT or GPL Version 2 licenses
	/*@cc_on(function(a,b){function r(a){var b=-1;while(++b<f)a.createElement(e[b])}if(!window.attachEvent||!b.createStyleSheet||!function(){var a=document.createElement("div");return a.innerHTML="<elem></elem>",a.childNodes.length!==1}())return;a.iepp=a.iepp||{};var c=a.iepp,d=c.html5elements||"abbr|article|aside|audio|canvas|datalist|details|figcaption|figure|footer|header|hgroup|mark|meter|nav|output|progress|section|subline|summary|time|video",e=d.split("|"),f=e.length,g=new RegExp("(^|\\s)("+d+")","gi"),h=new RegExp("<(/*)("+d+")","gi"),i=/^\s*[\{\}]\s*$/,j=new RegExp("(^|[^\\n]*?\\s)("+d+")([^\\n]*)({[\\n\\w\\W]*?})","gi"),k=b.createDocumentFragment(),l=b.documentElement,m=b.getElementsByTagName("script")[0].parentNode,n=b.createElement("body"),o=b.createElement("style"),p=/print|all/,q;c.getCSS=function(a,b){try{if(a+""===undefined)return""}catch(d){return""}var e=-1,f=a.length,g,h=[];while(++e<f){g=a[e];if(g.disabled)continue;b=g.media||b,p.test(b)&&h.push(c.getCSS(g.imports,b),g.cssText),b="all"}return h.join("")},c.parseCSS=function(a){var b=[],c;while((c=j.exec(a))!=null)b.push(((i.exec(c[1])?"\n":c[1])+c[2]+c[3]).replace(g,"$1.iepp-$2")+c[4]);return b.join("\n")},c.writeHTML=function(){var a=-1;q=q||b.body;while(++a<f){var c=b.getElementsByTagName(e[a]),d=c.length,g=-1;while(++g<d)c[g].className.indexOf("iepp-")<0&&(c[g].className+=" iepp-"+e[a])}k.appendChild(q),l.appendChild(n),n.className=q.className,n.id=q.id,n.innerHTML=q.innerHTML.replace(h,"<$1font")},c._beforePrint=function(){if(c.disablePP)return;o.styleSheet.cssText=c.parseCSS(c.getCSS(b.styleSheets,"all")),c.writeHTML()},c.restoreHTML=function(){if(c.disablePP)return;n.swapNode(q)},c._afterPrint=function(){c.restoreHTML(),o.styleSheet.cssText=""},r(b),r(k);if(c.disablePP)return;m.insertBefore(o,m.firstChild),o.media="print",o.className="iepp-printshim",a.attachEvent("onbeforeprint",c._beforePrint),a.attachEvent("onafterprint",c._afterPrint)})(this,document)@*/
	}

	//-----------------------------
	//3、修复ie6图片载入bug
	//-----------------------------
	try{document.execCommand("BackgroundImageCache", false, true);}catch(e){}

	//-----------------------------
	//4、Ajax不做缓存
	//-----------------------------
	$.ajaxSetup ({cache: false});
})();

/**
给Function的原型添加了两个方法
createDelegate（obj，args，appendArgs） obj 来改变function的this，args为传入的参数
*/
Function.prototype.createDelegate = function(obj, args, appendArgs){
	var method = this;
	return function() {
	    var callArgs = args || arguments;
	    if(appendArgs === true){
	        callArgs = Array.prototype.slice.call(arguments, 0);
	        callArgs = callArgs.concat(args);
	    }else if(typeof appendArgs == "number"){
	        callArgs = Array.prototype.slice.call(arguments, 0); // copy arguments first
	        var applyArgs = [appendArgs, 0].concat(args); // create method call params
	        Array.prototype.splice.apply(callArgs, applyArgs); // splice them in
	    }
	    return method.apply(obj || window, callArgs);
	};
};

/**
对Array原型添加了几个方法：
indexOf 索引数组中的元素
insertBefore 把新元素插入到目标元素之前
remove 从数组中删除元素
*/
Array.prototype.indexOf = function (o) {
	for (var i = 0; i < this.length; i++) {
		if (this[i] == o) {
			return i;
		}
	}
	return -1;
};

Array.prototype.insertBefore = function (o,o2) {
	var i = this.indexOf(o2);
	if (i == -1) {
		this.push(o);
	} else {
		this.splice(i, 0, o);
	}
};

Array.prototype.remove = function (o) {
	var i = this.indexOf(o);
	if (i != -1) {
		this.splice(i, 1);
	}
};


var T = {
	/**
	 * JS命名空间处理方法
	 */
	namespace : function() {
	    var a=arguments, o=null, i, j, d;
	    for (i=0; i<a.length; i=i+1) {
	        d=a[i].split(".");
	        o=T;
	
	        for (j=(d[0] == "T") ? 1 : 0; j<d.length; j=j+1) {
	            o[d[j]]=o[d[j]] || {};
	            o=o[d[j]];
	        }
	    }
	    return o;
	},
	
	/**
	 * JS实现继承处理方法
	 */
	extend : function(subc, superc, overrides) {
        var F = function() {};
        F.prototype=superc.prototype;
        subc.prototype=new F();
        subc.prototype.constructor=subc;
        subc.superclass=superc.prototype;

        if (superc.prototype.constructor == Object.prototype.constructor) {
            superc.prototype.constructor=superc;
        }
        
        if (overrides) {
            for (var i in overrides) {
                subc.prototype[i]=overrides[i];
            }
        }
	},
	
	/**
	 * 浅度复制变量
	 */
	apply : function(o, c) {
	    if(o && c && typeof c == 'object') {
	        for(var p in c){
	            o[p] = c[p];
	        }
	    }
	    return o;
	},
	
	/**
	 * 设置唯一索引值
	 */
	unique : 0,
	getUnique : function() {
		return "T-" + (++T.unique);
	},
	
	/**
	 * 获取Jquery对象
	 */
	get : function(el) {
		return (typeof el == "string" ? $("#"+el) : el);
	},
	
	/**
	 * 判断字符串是否是JSON类型
	 */
	isJson : function(text) {
		try {
			var json = eval("(" + text + ")");
			if (typeof json == 'xml') {
				return false;
			}
			return json;
		} catch(e) {
			return false;		
		}
	},
	
	/**
	 * 判断变量是否定义
	 */
	isDefined : function(a) {
		return (typeof a !== "undefined");
	},
	
	/**
	 * 空函数，在函数为空的的情况下使用
	 */
	emptyFn :function() {
		
	},
	/**
	 * 是否是手机浏览器
	 */
	mobile :false,

	/**
	 * 获取随即字符
	 */
	randStr : function(type, len) {
		var pool = "0123456789";
	    if (type == "alpha") pool = "abcdefghijklmnopqrstuvwxyz";

	    var str = "", pl = pool.length;
	    
		for (var i = 0; i < len; i++) {
			str += pool.charAt(Math.ceil(Math.random() * 100000000) % pl);
		}
		
		return str;
	},
	
	focus :function(id){
	  	var input = (typeof id=="string")?$("#"+id):id;
	  	if($.trim(input.val()) == '') {
	  		$(function(){input.focus();});
	  	}
	},
	
	refresh : function(imgId, inputId, url, isFocus) {
		if(!url) {
			url = $("#" + imgId).attr("src");
		}
		if (url.indexOf("?") == -1) {
			$("#" + imgId).attr({src:url + "?t=" + Math.random()});
		} else {
			$("#" + imgId).attr({src:url + "&t=" + Math.random()});
		}
		
		var input = $("#" + inputId).val("");
		
		if (isFocus) {
			input.focus();
		}
	},
	
	/**
	 * 提示Ajax请求错误
	 */
	showAjaxReqError : function(request, url, message) {
	  	if (request.status == 0) {
			return;
		}
		
  		if (request.status == 403) {			
			var json = T.isJson(request.responseText);			
			if( json && json.resp_code == 403 ) {
				alert("对不起，登录超时，请重新登录");
				window.location.href= (url || "/a/login.do");
				return true;
			}
		} else if(request.status == 404) {
			alert("对不起，该请求不存在，或者已删除。");
		} else {
			alert(message || "对不起，请求超时，请重新连接");
		}
    },
    
    /**
     * 提示Ajax form 错误信息提示
     */
    showAjaxFormError : function(formV, json) {
  		for(var name in json) {
			var field = formV.form.find("input[name="+name+"]");
  			formV.setStatus("toError", field, json[name]);
		}
    },
    
    /**
     * 获取单例body mask
     */
    _singleMask : null,
    getMask : function() {
    	if(!T._singleMask) {
    		T._singleMask = new T.Mask();
    	}
    	return T._singleMask;
	},
	
	/**
	 * 获取单例的loadTip
	 */
	_singleLoadTip : null,
	getLoadTip :function() {
		if(!T._singleLoadTip) {
			T._singleLoadTip = new T.LoadTip();
		}
		return T._singleLoadTip;
	},
	
	/**
	 * 获取单例的Tip
	 */
	_singleTip :null,
	getTip :function() {
		if(!T._singleTip) {
			T._singleTip = new T.Tip();
		}
		return T._singleTip;
	},
	
	/**
	 * 获取padding值和margin值
	 */
	getBox :function(el, attr) {
		
		if(attr == "tb")
		{
			return T.getStyleVal(el, "paddingTop") + T.getStyleVal(el, "marginTop") + T.getStyleVal(el, "paddingBottom") + T.getStyleVal(el, "marginBottom");
		}
		else if(attr == "lr")
		{
			return T.getStyleVal(el, "paddingLeft") + T.getStyleVal(el, "marginLeft") + T.getStyleVal(el, "paddingRight") + T.getStyleVal(el, "marginRight");
		}		
	},
	
	/**
	 * 获取css值
	 */
	getStyleVal :function(el, attr) {
		var val = el.css(attr);
			val = val.replace("px", "");
			val = parseInt(val);
			
			if(isNaN(val)) {
				val = 0;
			}
			return val;
	},
	/**
	 * 设置光标位置函数
	 */
	setCaretPosition :function(ctrl, pos){
		try{
			if(ctrl.setSelectionRange)
			{
				ctrl.focus();
				ctrl.setSelectionRange(pos,pos);
			}
			else if (ctrl.createTextRange) {
				var range = ctrl.createTextRange();
				range.collapse(true);
				range.moveEnd('character', pos);
				range.moveStart('character', pos);
				range.select();
			}
		}catch(e){
		}
	},
	
	resizePage :function(maxWidth) {

		if(screen.width <= 1024)
		{
			return ;
		}
		
		maxWidth = maxWidth || 1024;
		maxWidth = maxWidth * (screen.width / 1024);
		
		var resetWidth = function() 
		{
			var docWidth = $(document).width();
			
			if(docWidth >= maxWidth)
			{
				$(document.body).css("width", maxWidth);
			}
			else
			{
				if(docWidth < 800)
				{
					$(document.body).css("width", 800);
				} else {
					$(document.body).css("width", "auto");
				}
			}			
		};
		
		$(window).resize(resetWidth);
		$(document).ready(resetWidth);
	},
	
	execScript :function(ajaxLoadedData) {
		
		// 第一步：匹配加载的页面中是否含有js
		var regDetectJs = /<script(.|\n)*?>(.|\n|\r\n)*?<\/script>/ig;
		var jsContained = ajaxLoadedData.match(regDetectJs);

		// 第二步：如果包含js，则一段一段的取出js再加载执行
		if(jsContained) {
			// 分段取出js正则
			var regGetJS = /<script(.|\n)*?>((.|\n|\r\n)*)?<\/script>/im;

			// 按顺序分段执行js
			var jsNums = jsContained.length;
			for (var i=0; i<jsNums; i++) {
				var jsSection = jsContained[i].match(regGetJS);

				if(jsSection[2]) {
					if(window.execScript) {
						// 给IE的特殊待遇
						window.execScript(jsSection[2]);
					} else {
						// 给其他大部分浏览器用的
						window.eval(jsSection[2]);
					}
				}
			}
		}
	}
};

(function() {
	var ua = navigator.userAgent;
	
	// WebKit
    if (/AppleWebKit\/([\d.]*)/.test(ua)) {

    	// Apple Mobile
        if (/ Mobile\//.test(ua)) {
            T.mobile = true; // iPad, iPhone or iPod Touch
        }
        
        // Other WebKit Mobile Browsers
        else if (ua.match(/NokiaN[^\/]*|Android \d\.\d|webOS\/\d\.\d/)) {
            T.mobile = true; // Nokia N-series, Android, webOS, ex: NokiaN95
        }
    }
	if(T.mobile) {
		$(function(){
			$(document.documentElement).addClass("t_mobile");
		});
	}
})();

T.namespace("Util");

/**
 * 观察者类
 * 主要用来处理事件机制的, 所有有需要使用到事件处理的组件都继承于这个观察者类
 */
T.Util.Observer = function() {
	if(!this.listeners) {
		this.listeners = {};
	}
}
T.Util.Observer.prototype = {
	addEvent :function(event) {
		for(var eventName in event) {
			this.listeners[eventName] = new T.Util.CustomEvent(eventName, this);
		}
	},
	on :function(eventName,fn) {
		this.listeners[eventName].on(fn);
	},
	un :function(eventName,fn) {
		return this.listeners[eventName].un(fn);
	},
	fireEvent :function(eventName,args) {
		this.listeners[eventName].fire(args);
	},
	unsubscribeAll :function() {
		this.listeners[eventName].unsubscribeAll();
	}
};

T.Util.CustomEvent = function(type, oScope) {

    this.type = type;
	this.subscribers = [];
    this.scope = oScope || window;
}
T.Util.CustomEvent.prototype = {
	on: function(fn) {
		this.subscribers.push(new T.Util.Subscriber(fn, this.scope));
	},
	un: function(fn) {
		var found = false;
        for (var i=0, len=this.subscribers.length; i<len; ++i) {
            var s = this.subscribers[i];
            if (s && s.contains(fn, s.scope)) {
                this._delete(i);
                found = true;
            }
        }
        return found;
	},
	fire: function(args) {
		 args = args || [];
		 var length = this.subscribers.length;
		 try {
			 for(var i=0;i<length;i++) {
			 	var s = this.subscribers[i];
			 	s.fn.apply(s.obj, args);
			 }
		 } catch(e) {
		 	return false;
		 }
		 return true;
	},
	unsubscribeAll: function() {
		for (var i=this.subscribers.length-1; i>-1; i--) {
            this._delete(i);
        }
		
        this.subscribers=[];
        return i;
	},
	_delete :function(index) {
		var s = this.subscribers[index];
        if (s) {
            delete s.fn;
            delete s.obj;
        }
        this.subscribers.splice(index, 1);
	}
};

/**
 * 订购者处类，主要用来记录需要 被监听的的方法
 */
T.Util.Subscriber = function(fn, obj) {
    this.fn = fn;
    this.obj =  obj || null;
};

T.Util.Subscriber.prototype = {
	contains : function(fn, obj) {
	    if (obj) {
	        return (this.fn == fn && this.obj == obj);
	    } else {
	        return (this.fn == fn);
	    }
	}
};

/**
 * 简单事件 监听处理Bus
 */
T.Util.Bus = {
	listener :{},
	add :function(name, fn){
		var events = T.Util.Bus.listener[name] || [];
			events.push(fn);
			T.Util.Bus.listener[name] = events;	
	},
	fire :function(name, args){
		var events = T.Util.Bus.listener[name] || [];
		for(var i=0;i<events.length;i++){
			if(events[i]){
				events[i](args);
			}
		}
	}
};

T.namespace("Form");

/**
 * 表单校验控件
 * 配置参数
 * config = {
 * 		submitText : "提交后按钮显示文字",
 * 		disCls : "提交后按钮disabled状态",
 * 		onsubmit : "提交后，校验表单成功后触发函数",
 * 		scrollToError : "是否滚动到错误点",
 * 		errorBreak : "错误时，直接停止校验"
 * }
 */
T.Form.Validator = function(el, config) {
	this.form = T.get(el);
	this.config = config;
	
	T.Form.Validator.superclass.constructor.call(this);
	this.init();
}

T.extend(T.Form.Validator, T.Util.Observer, {
	init : function() {
		this.valids = {},
		this.form.bind("submit", this.submit.createDelegate(this));
	},	
	submit : function(e) {
		
		if(this.executeAll() !== true) {
			e.stopPropagation();
			e.preventDefault();
		} else {
			if (typeof this.config.submitText != "undefined") {
				var submitBtn = this.form.find("input[type=submit]");
					submitBtn.prop("disabled", true);
					if(this.config.disCls) {
						if(this.config.fixIEBorder) {
							submitBtn.parent().addClass(this.config.disCls);
						} else {
							submitBtn.addClass(this.config.disCls);
						}
					}
					submitBtn.val(this.config.submitText);
			}
		}
	},
	add : function(name, rule, params) {
		params = params || {};
		var valid = {id:name, rule:rule, params:params};
		if(this.valids[name]) {
			this.valids[name].push(valid);
		} else {
			this.valids[name] = [valid];
			this.addValidEvent(valid);
		}
	},
	remove : function() {
		this.valids[id] = null;
		delete this.valids[id];
	},
	execute :function(type, valids) {
		
		var validObj = null;
		var value = "";
		if(valids.length > 0) {
			validObj = this.getValidObj(valids[0]);
			value = $.trim(validObj.val());
		}
		
		for(var i = 0 ; i < valids.length ; i++) {
			if(validObj && validObj.length == 0){
				break;
			}
			var valid = valids[i];
			var result = valid.rule(type, value, validObj, valid.params);
			
			if(result !== true) {
				this.setStatus("toError", validObj, result);
				if(type == "submit") {
					this.scrollToObj(validObj);
				}
				return false;
			}
		}
		
		this.setStatus("toHelp", validObj);
		
		return true;
	},
	executeAll : function() {
		
		//清空错误表单项
		this.scrollToErrorInput = null;
		
		var flag = true;
		for(var id in this.valids) {
			var result = this.execute("submit", this.valids[id]);
			
			if(result !== true) {
				flag = false;
				if(this.config.errorBreak){
					break;
				}
			}
		}
		
		if(flag !== true) {
			return false;
		}
		
		return (typeof this.config.onsubmit=="function") ? this.config.onsubmit(this) : true;
	},
	scrollToObj :function(input) {
		if(this.config.scrollToError !== true) {
			return false;
		}
		
		if(input && this.scrollToErrorInput == null) {
			this.scrollToErrorInput = input;
			var inputTop = input.offset().top;
			if(document.documentElement.scrollTop > inputTop) {
				window.scrollTo(0, input.offset().top - 10);
			}
		}
		
	},
	addValidEvent : function(valid) {
		
		var self = this;
		var validObj = this.getValidObj(valid);
		
		var tagName = validObj.get(0) ? validObj.get(0).tagName.toUpperCase() : "";
		
		if(tagName == "INPUT" || tagName == "TEXTAREA") {
			validObj.bind("focus",function() {
				self.setStatus("toFocus", validObj);
			}).bind("blur",function() {
				self.execute("blur", self.valids[valid.id]);
			});
			if(valid.params.keyup === true) {
				validObj.bind("keyup",function(){
					self.execute("keyup", self.valids[valid.id]);
				});
			}
		} else if(tagName=="SELECT") {
			validObj.bind("change",function() {
				self.execute("change", self.valids[valid.id]);
			});
		}
	},
	getValidObj : function(valid) {
		var obj = this.form.find("[name='" + valid.id + "']");
		return obj;
	},
	setStatus :function(status, validObj, tips) {
		
		if(!validObj.get(0)){return;}
		
		//单独一个地方错误提示
		if(this.config && this.config.errorShow && this.config.errorBreak === true) {
			this.config.errorShow(status, tips);
			return;
		}
		
		var fieldObj = validObj.parents("div.row:first");
		var	helpObj = fieldObj.find(".note");
		var errorObj = fieldObj.find(".warning");
		
		var isSuccess = fieldObj.hasClass("t_success");
			fieldObj.removeClass("t_error").removeClass("t_focus").removeClass("t_help").removeClass("t_success").removeClass("t_check");
		
		switch(status)
		{
			case "toHelp" :
				fieldObj.addClass("t_help");
				if(tips) {
					helpObj.text(tips);
				}
				errorObj.empty();
				
				if(isSuccess) {
					fieldObj.addClass("t_success");
				}
				break;
				
			case "toError" :
				errorObj.html(tips);			
				fieldObj.addClass("t_error");
				break;
				
			case "toFocus" :
				if(isSuccess){
					fieldObj.addClass("t_success");
				}
				fieldObj.addClass("t_focus");
				break;
				
			case "toSuccess" :
				fieldObj.addClass("t_success");
				break;
				
			case "toCheck" :
				fieldObj.addClass("t_check");
				break;
		}
	}
});


//-----------------------------
//公共校验方法
//-----------------------------
T.Form.Rule = {
	Format :function(type, val, el, oParams) {
		if(val != "") {
			if(!oParams.reg.test(val)) {
				return oParams.tips;
			}
		}
		return true;
	},
	Not :function() {
		return true;
	},
	EmailReg : /^([^@\s]+)@((?:[-a-z0-9]+\.)+[a-z]{2,})$/i,
	Email : function(type, val, el, oParams) {
		if(val.length > 50) {
			return "邮箱地址不能超过50个字符";
		}
		oParams.reg = T.Form.Rule.EmailReg;
		return T.Form.Rule.Format(type, val, el, oParams);		
	},
	MobileReg :/^1[3458]\d{9}$/,
	Mobile :function(type, val, el, oParams) {
		oParams.reg = T.Form.Rule.MobileReg;
		return T.Form.Rule.Format(type, val, el, oParams);		
	},
	PhoneReg :/^\d{3,4}\-?\d{7,8}$/,
	Phone :function(type, val, el, oParams) {
		oParams.reg = T.Form.Rule.PhoneReg;
		return T.Form.Rule.Format(type, val, el, oParams);		
	},
	PhoneAndMobile :function(type, val, el, oParams) {
		if(val != "") {
			if(T.Form.Rule.MobileReg.test(val) || T.Form.Rule.PhoneReg.test(val)) {
				return true;
			} else { 
				return oParams.tips;
			}
		}
		return true;
	},
	ZipReg :/^\d{6}$/,
	Zip :function(type, val, el, oParams) {
		oParams.reg = T.Form.Rule.ZipReg;
		return T.Form.Rule.Format(type, val, el, oParams);
	},
	QQ	:function(type,val,el,oParams) {
		if(val != "") {
			if(!T.Form.Rule.EmailReg.test(val) && !(/^\d{5,12}$/.test(val))) {
				return oParams.tips;
			}
		}
		return true;
	},
	Digital :function(type,val,el,oParams) {
		oParams.reg = /^[0-9]+$/;
		return T.Form.Rule.Format(type, val, el, oParams);
	},
	EnAndDigitalReg :/^[A-Za-z0-9]+$/,
	EnAndDigital :function(type, val, el, oParams) {
		oParams.reg = T.Form.Rule.EnAndDigitalReg;
		return T.Form.Rule.Format(type, val, el, oParams);
	},
	RmbReg :/^(([1-9]\d*)|0)(\.\d{1,2})?$/,
	Rmb :function(type, val, el, oParams) {
		oParams.reg = T.Form.Rule.RmbReg;
		return T.Form.Rule.Format(type, val, el, oParams);
	},
	ImgReg :/\.(?:png|jpg|gif|jpeg)$/i,
	Img :function(type, val, el, oParams) {
		oParams.reg = T.Form.Rule.ImgReg;
		return T.Form.Rule.Format(type,val,el,oParams);
	},
	Length :function(type, val, el, oParams) {
		if(val == "") {
			return true;
		}
		
		var length = val.length;
		
		if("submit" == type || "blur" == type) {
			if(oParams.maxLength){
				if(length > oParams.maxLength) {
					return oParams.tips || oParams.maxTips;
				}
			}
			if(oParams.minLength) {
				if(length < oParams.minLength) {
					return oParams.tips || oParams.minTips;
				}
			}
		}		
		return true;
	},
	Presence :function(type, val, el, oParams) {
		
		if("submit" == type) {
			if(val=="") {
				return oParams.tips;
			}
			if(el.get(0).tagName.toUpperCase()=="SELECT" && (val == -1 || val == "")) {
				return oParams.tips;
			}
		}
		
		return true;
	},
	ZwReg :/^[\u4E00-\u9FA5]+$/,
	Zw :function(type, val, el, oParams) {
		oParams.reg = T.Form.Rule.ZwReg;
		return T.Form.Rule.Format(type, val, el, oParams);
	},
	Account :function(type, val, el, oParams) {
		
		if(val == "") {
			if(type == "blur") return true;
			return "请输入用户账号";
		}
		
		if(!val.match(/^[a-zA-Z]/)) {
			return "请输入首位为字母的用户账号";
		}
		
		if(!val.match(T.Form.Rule.EnAndDigitalReg)) {
			return "请输入仅由字母和数字组成的用户账号 ";
		}
		return true;
	},
	DateCheck :function(type, val, el, oParams) {
		if (val == "") {
			return true;
		}
		
		if(!/((^((1[8-9]\d{2})|([2-9]\d{3}))([-\/\._])(10|12|0?[13578])([-\/\._])(3[01]|[12][0-9]|0?[1-9])$)|(^((1[8-9]\d{2})|([2-9]\d{3}))([-\/\._])(11|0?[469])([-\/\._])(30|[12][0-9]|0?[1-9])$)|(^((1[8-9]\d{2})|([2-9]\d{3}))([-\/\._])(0?2)([-\/\._])(2[0-8]|1[0-9]|0?[1-9])$)|(^([2468][048]00)([-\/\._])(0?2)([-\/\._])(29)$)|(^([3579][26]00)([-\/\._])(0?2)([-\/\._])(29)$)|(^([1][89][0][48])([-\/\._])(0?2)([-\/\._])(29)$)|(^([2-9][0-9][0][48])([-\/\._])(0?2)([-\/\._])(29)$)|(^([1][89][2468][048])([-\/\._])(0?2)([-\/\._])(29)$)|(^([2-9][0-9][2468][048])([-\/\._])(0?2)([-\/\._])(29)$)|(^([1][89][13579][26])([-\/\._])(0?2)([-\/\._])(29)$)|(^([2-9][0-9][13579][26])([-\/\._])(0?2)([-\/\._])(29)$))/.test(val)) {
			return "请输入有效的日期，格式为2000-01-01";
		}
		
		if(new Date() < new Date(Date.parse(val.replace(/-/g, "/")))) {
			return "请输入有效的日期，格式为2000-01-01";
		}
		
		return true;
	},
	IdCard :function(type, id, el, oParams) {

		 if(id == '') {
	 		 return true;
	 	 }
		 if(type !== "submit" && type !== "blur") {
		 	return true;
		 }
		 if(id.match(/^\d{6}(19|20)\d{2}[01]\d{1}[0-3]\d{1}(\d{4}|\d{3}x)$/i)) {
			 var weight = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2];
			 var checkCode = [1, 0, 'x', 9, 8, 7, 6, 5, 4, 3, 2];
			 var s = 0;
			 for(var i=0; i<17; i++)
			 {
			 	s += id.substr(i, 1) * weight[i];
			 }
			 if(checkCode[s%11] != id.substr(17, 1).toLowerCase( )) {
				 return oParams.tips;
			 }
			 var year = parseInt(id.substr(6,4),10);
			 var month = parseInt(id.substr(10,2),10);
			 var day = parseInt(id.substr(12,2),10);
			 var date=new Date(month + '/' + day + '/' + year);
			 if(date.getFullYear()!=year || date.getDate()!=day || date.getMonth()!=(month-1))
			 {
				 return oParams.tips;
			 }
		 } else if(id.match(/^\d{6}\d{2}[01]\d{1}[0-3]\d{1}(\d{3}|\d{2}x)$/i)) {
			 var year = parseInt('19'+id.substr(6,2),10);
			 var month = parseInt(id.substr(8,2),10);
			 var day = parseInt(id.substr(10,2),10);
			 var date=new Date(month + '/' + day + '/' + year);
			 if(date.getFullYear()!=year || date.getDate()!=day || date.getMonth()!=(month-1)) {
				 return oParams.tips;
			 }
		 } else {
			 return oParams.tips;
		 }
		 
		 return true;
	},
	Gt10 :function(type, val, el, oParams) {
		oParams.reg = T.Form.Rule.RmbReg;
		var result = T.Form.Rule.Format(type,val,el,oParams);
		if(result === true) {
			var rmb = parseInt(val);
			if(rmb < 10) {
				return oParams.tips;
			}
		} else {
			return result;
		}
		return true;
	},
	Captcha :function(type, val, el, oParams) {
		
		var statusObj,emObj;
		
		if(oParams.statusId) {
			statusObj = $("#" + oParams.statusId);
			emObj = statusObj.find("em").removeClass("reTrue").removeClass("reFalse");
		}
		
		if(type == "blur" && val == "") {
			
			return true;
		} else if(val == "") {
			
			return "请输入验证码";
		} else if(val.length != 4) {
			
			return "验证码输入错误";
		}
		
		if(type == "blur") {
			if(!oParams.url) {
				return true;
			}
			
			emObj ? emObj.addClass("loading") : null;
			
			$.ajax({
				url:oParams.url,
				data:{'ta_code' : val, 'ta_position': oParams.position},
				dataType :"json",
				type :"POST",
				success:function(data) {
					if(!data) return;
					if(data.resp_code == 0) {
						emObj ? emObj.removeClass("loading").addClass("reTrue") : null;
					} else {
						emObj ? emObj.removeClass("loading").addClass("reFalse") : null;
						T.Form.Validator.prototype.setStatus("toError", el, oParams.tips);
					}
				},
				error :function() {
					emObj ? emObj.removeClass("loading") : null;
				}
			});
		} else if (type == "submit") {
			if(emObj && emObj.hasClass("reFalse")) {
				return oParams.tips;
			}
		}
		
		return true;
	}
};



T.namespace( "Util", "Data" );

T.Util.Detect = function() {
	this.ls = 0; // flag for supported of localStorage
};

T.Util.Detect.prototype = {
	// if localStorage is supported.
	localStorage : function() {
		
		if (0 != this.ls) return( 1 == this.ls );
		
		var uid = new Date,
	        storage,
	        result;
		
	    try {
	    	
		    (storage = window.localStorage).setItem(uid, uid);
		    result = storage.getItem(uid) == uid;
		    storage.removeItem(uid);
		    this.ls = (result && storage) ? true : false;
		    
	    } catch(e) {this.ls = false;}
	    
	    return this.ls;
	}
};


T.Util.JsonAdapter = function () {};

T.Util.JsonAdapter.prototype = {
	
	encode : function( value ) {
		//return JSON.stringify( value );
		return value;
	}, 
	
	decode : function( value ) {
		//return JSON.parse( value );
		return value;
	}
};

T.Data.IEStorage = function() {
	
	this.userdata = null;
	this.name = location.hostname;
    this.expires = 365;
	this.isSupported = false;
	
	try {
    	
        this.userdata = document.createElement("INPUT");
        this.userdata.type = "hidden";
        this.userdata.addBehavior( "#default#userData" );
        
        document.body.appendChild( this.userdata );
        
        this.isSupported = true;
    } catch(e) {
    	this.isSupported = false;
    }
};

T.Data.IEStorage.prototype = {
		
	supported : function() { return this.isSupported; },
    
	// Clear the storage.
	clear: function() { return this.flush( -1 ); },

	// Get an item from the storage.
	getItem : function( key ) {
		
		this.userdata.load( this.name );
        return this.userdata.getAttribute( key );
	},
	
	// Remove the given item from the storage.
	removeItem: function( key ) {
		
		this.userdata.load( this.name );
        this.userdata.removeAttribute( key );
        
	},

	// Store the item in the storage.
	setItem: function( key, value ) {
		
		this.userdata.load( this.name );
        this.userdata.setAttribute( key, value );
        this.flush();
	},

	// Save thi storage
    flush : function( expires ) {
        
        var nowDate = new Date();
        nowDate.setDate(nowDate.getDate() + (expires || this.expires));
        this.userdata.expires = nowDate.toUTCString();
    	this.userdata.save( this.name );
    }
};

T.Util.Cache = function( adapter ) {
	
	// Set adapter of data which implements adapter interface.
	if (T.isDefined( adapter )) this.adapter = adapter;
	else this.adapter = new T.Util.JsonAdapter();
	
	// Set storage of cache which implements storage interface.
	var detect = new T.Util.Detect();

	if ( detect.localStorage() ) {
		this.storage = window.localStorage;
	} else {
		this.storage = new T.Data.IEStorage();
		
		if ( !this.storage.supported() ) {
			
			delete this.storage;
		}
	}
};

T.Util.Cache.prototype = {
		
	supported : function() { return T.isDefined( this.storage ); },
	
	// Clear the cache.
	clear: function() {
		if ( this.supported() ) this.storage.clear();
		return( this );
	},

	// Get an item from the cache. If the item cannot be
	// found, I can pass back an optional default value.
	getItem : function( key, def ) {
		
		if ( !this.supported() ) return false;
		
		var value = this.storage.getItem( key );
		
		var time = new Date();
	    if ( time.getTime() - this.storage.getItem( "meta_ct_" + key ) > this.storage.getItem("meta_lt_" + key) )
	    	return false;
		
	    if (null == value) {
			return( T.isDefined( def ) ? def : null );
		} else {
			return( this.adapter.decode( value ) );
		}
	},

	// Check to see if the given key exists in the storage
	// container.
	hasItem: function( key ) {
		if ( !this.supported() ) return false;
		return( null != this.storage.getItem( key ) );
	},
	
	// Remove the given item from the cache.
	removeItem: function( key ) {
		if ( this.supported() ) this.storage.removeItem( key );
		return( this );
	},

	// Store the item in the cache. When doing this, 
	// automatically encode the value.
	setItem: function( key, value, expire ) {
		expire = expire || new Date().getTime() + 3600000 * 24 * 10;
		if ( this.supported() ) {
			
			this.storage.setItem( key, this.adapter.encode( value ) );
			
			var time = new Date();
			this.storage.setItem( "meta_ct_" + key, time.getTime() );
			this.storage.setItem( "meta_lt_" + key, expire );
		}
		
		return( this );
	}
};


/**
 * 遮罩层组件
 */
T.namespace("Mask");
T.Mask = function(parent, config) {
	
	parent = parent || $(document.body);
	this.parent = T.get(parent);
	this.config = config || {};
	T.Mask.superclass.constructor.call(this);
};

T.extend(T.Mask, T.Util.Observer, {
	
	rendered : false,
	init :function() {
		
		if(this.rendered === true) {
			return ;
		}
		this.rendered = true;
		
		this.initDom();
		this.initEvent();
	},
	initDom :function() {
		var html = [
				'<div class="t_mask">',
					'<iframe frameborder="0"></iframe>',
					'<div class="t_mask_opacity" style="opacity:0.4;filter:alpha(opacity=40)"></div>',
					(this.config.isLoading?'<div class="t_mask_text">加载中，请稍后...</div>':""),
				'</div>'
			].join("");
			
		this.el = $(html);
		this.el.css({left:-10000, top:-10000});
		this.parent.append(this.el);
	},
	initEvent :function() {
		$(window).bind("resize", this.onReszie.createDelegate(this));
	},
	isShow :function() {
		if(this.el && this.el.css("top") == "0px") {
			return true;
		}
		return false;
	},
	show :function() {		
		this.init();
		this.el.css({top:0, left:0});
		this.el.height($(document).height());
		this.el.css({display:"block"});
	},
	hide :function() {
		this.el.css({top:-10000,left:-10000});
	},
	onReszie :function() {
		//修复ie浏览器的bug
		if(this.el && this.el.css("display") == "block" && this.el.css("top") == "0px") {
			this.el.css({top:0, left:0});
			this.el.height($(document).height());
		}
	}
});


/**
 * 弹出层组件
 */
T.namespace("Layer");
T.Layer = function(el, config) {

	this.el = T.get(el);
	this.config = config || {};

	if (this.el.parent().get(0) != document.body) {
		$(document.body).append(this.el);
	}
	
	T.Layer.superclass.constructor.call(this);
	this.addEvent({
		"hide" : true
	});
}

T.extend(T.Layer, T.Util.Observer, {

	readered : false,
	init : function() {
		if (this.readered) {
			return;
		}
		this.readered = true;
		this.initDom();
		this.initEvent();
		
		if(this.config.mask !== false) {
			this.mask = T.getMask();
		}
	},
	initDom : function() {
		this.initDomImp();
	},
	initEvent : function() {
		this.el.find(".close_layer").click(this.hide.createDelegate(this));
		
		var self = this;
		$(window).bind('resize', function() {
			self.reSetPosition.call(self);
		});
	},
	initDomImp : function() {
		// 初始化DOM 实现
	},
	showImp : function(params) {
		// 显示实现
	},
	hideImp : function() {
		// 隐藏实现
	},
	show : function(params) {
		this.init();
		
		this.showImp(params);
		this.reSetPosition();
		this.el.css("visibility", "visible");

		if (this.config.mask !== false && this.mask) {
			this.mask.show();
		}

		if (this.config.escKey !== false) {
			this.addEscKey();
		}
	},
	hide : function() {
		this.el.css("visibility", "hidden");
		if (this.config.mask !== false && this.mask) {
			this.mask.hide();
		}
		this.hideImp();
		this.fireEvent("hide", this);
	},
	reSetPosition : function() {
		var top = (($(window).height() / 2) - (this.el.outerHeight() / 2));
		var left = (($(window).width() / 2) - (this.el.outerWidth() / 2)) + 0;
		if (top < 0) {top = 0;}
		if (left < 0) {left = 0;}
		
		top = top + $(window).scrollTop();
		
		if($.browser.msie){
			this.el.css({top : top, left : left});
		} else {
			this.el.css({top : top, left : left});
		}
	},
	addEscKey : function() {
		var self = this
		var onKeyUp = function(e) {
			if (e.keyCode == 27) {
				if (e.target && e.target.type == "file") {
					return;
				}
				self.hide();
				$(document.body).unbind("keyup", onKeyUp);
			}
		}
		$(document.body).bind("keyup", onKeyUp);
	}
});

/**
 * 在页面中显示Frame层
 */
var FrameLayer = function() {
	var layerHtml = [
			'<div id="j_iframe_layer" class="t_layer">',
				'<div class="layer_body">',
					'<div class="layer_title">',
						'<h3></h3>',
						'<a class="close_layer" href="javascript:;">X 关闭</a>',
					'</div>',
					'<div class="layer_cont">',
					'</div>',
				'</div>',
			'</div>'
	    ];
	var el = $("#j_iframe_layer");
	this.el = el.length > 0 ? el :$(layerHtml.join(""));
	FrameLayer.superclass.constructor.call(this, this.el, {});
}
T.extend(FrameLayer, T.Layer, {
	
	initDomImp :function() {
		this.title = this.el.find(".layer_title h3");
		this.cont  = this.el.find(".layer_cont");
	},
	showImp :function(params) {
		
		this.setTitle(params.title);
		this.setWidth(params.width);
		this.setHeight(params.height);
		
		if(typeof params.url === "string") {
			if(this.cont.html() == "") {
				this.cont.append("<iframe src='" + params.url + "' frameborder='0' scrolling='no'></iframe>");
			}
			
			if($.browser.msie) {
				this.cont.find("iframe").attr("src", params.url);
			}
			
			this.setFrameWidth(params.width);
			this.setFrameHeight(params.height);
		} else {
			var callback = params.url;
				params.callback = null;
				delete params.callback;
				
			if(typeof callback === "function") {
				callback.call(this);
			}
		}
	},
	hideImp :function() {
		if(this.params && typeof this.params.url === "string") {
			if($.browser.msie) {
				this.cont.find("iframe").attr("src", "");
			} else {
				this.cont.empty();
			}
		} else {
			this.params = null;
			delete this.params;
			this.cont.empty();
		}
	},
	setTitle :function(title) {
		this.title.text(title);
	},
	setWidth :function(width) {
		if(width)
		{
			this.el.css({width: width + 20});
		} 
		else 
		{
			this.el.css({width: "auto"});
		}
	},
	setFrameWidth :function(width) {
		this.cont.find("iframe").css({width: width});
	},
	setHeight :function(height) {
		if(height)
		{
			height = height + 32 + 20;
			if(!$.browser.msie) {
				height += 4;
			}
			this.el.css({height: height});
		}
		else 
		{
			this.el.css({height: "auto"});
		}
	},
	setFrameHeight :function(height) {
		this.cont.find("iframe").css({height: height});
	},
	reset :function(frame) {
		
		if(frame.contentDocument){
			var height = frame.contentDocument.body.offsetHeight;
			
			if(height > 0) {
				this.setHeight(height);
				this.setFrameHeight(height);
			}	
			var width = frame.contentDocument.body.offsetWidth;
			if(width > 0) {
				this.setWidth(width);
				this.setFrameWidth(width);
			}
		}else if(frame.Document && frame.Document.body.scrollHeight){
			
			this.setHeight(frame.Document.body.scrollHeight);
			this.setFrameHeight(frame.Document.body.scrollHeight);
			
			this.setWidth(frame.Document.body.scrollWidth);
			this.setFrameWidth(frame.Document.body.scrollWidth);
		}
		
		this.reSetPosition();
	},
	confirm :function(text, submit, cancel) {
		var self = this;
		this.cont.append("<div class='layer_text'>"+text+"</div><div class='layer_btn'><span class='btn04 btn'><input type='submit' value='  确  认  ' class='btn_srch' name='ts_submit'></span><span class='btn05 btn'><input type='submit' value='  取  消  ' class='btn_srch' name='ts_submit'></span></div>");
		this.cont.find(".btn04").click("click", function(){
			submit(self);
			self.hide();
		});
		this.cont.find(".btn05").click("click", function(){
			cancel(self);
			self.hide();
		});
	},
	alert :function(text, submit, cancel) {
		var self = this;
		this.cont.append("<div class='layer_text'>"+text+"</div><div class='layer_btn'><span class='btn04 btn'><input type='submit' value='  确  认  ' class='btn_srch' name='ts_submit'></span></div>");
		this.cont.find(".btn05").click("click", function(){
			cancel(self);
			self.hide();
		});
	}
});

/**
 * loading 提示
 */
T.namespace("LoadTip");

T.LoadTip = function(parent) {

	parent = parent || $(document.body);
	
	this.el = $("<div class='t_tip01'><div><span>数据加载中</span></div></div>");
	
	T.get(parent).append(this.el);
	
	T.LoadTip.superclass.constructor.call(this);
};

T.extend(T.LoadTip, T.Util.Observer, {
	layout :function() {
		
		var top = ((this.el.parent().outerHeight() / 2) - (this.el.outerHeight() / 2));
		var left = ((this.el.parent().outerWidth() / 2) - (this.el.outerWidth() / 2));
		
		if (top < 0) {top = 0;}
		if (left < 0) {left = 0;}

		top = top + $(window).scrollTop();
			
		this.el.css({top : top, left : left});
	},
	show :function(text, type) {
		var div = this.el.find("div");
			div.removeClass("loading").removeClass("success").removeClass("failure");
			div.addClass(type || "loading");
			
		this.el.find("span").text(text || "数据加载中");
		this.layout();
		this.el.css("visibility", "visible");
		
		if(type == "success") {
			var self = this;
			setTimeout(function(){
				self.el.fadeOut("slow", function(){
					self.hide();
					self.el.css("display", "");
				});				
			}, 3000);
		}
	},
	hide :function() {
		this.el.css("visibility", "hidden");
	}
});

/**
 * 淡入淡出提示
 */
T.namespace("Tip");
T.Tip = function() {
	
	var el = T.get("j_t_tip02");
	if(el.length == 0) {
		el = $("<div class='t_tip02' id='j_t_tip02'><div><span>数据加载中</span></div></div>");
		$(function(){
			$(document.body).append(el);
		});
	}
	this.el = el;
	
	T.Tip.superclass.constructor.call(this);
};
T.extend(T.Tip, T.Util.Observer, {
	layout :function() {
		var top = $(window).scrollTop();
		var left = (($(window).width() / 2) - (this.el.outerWidth() / 2));
		this.el.css({top: -top - this.el.height() - 20, left: left});
	},
	show :function(text, status) {
		this.el.removeClass("t_tip02_success").removeClass("t_tip02_failure").addClass("t_tip02_"+status);
		this.el.find("span").text(text || "数据加载中");
		
		this.layout();
		var top = $(window).scrollTop();
		var self = this;
		
		this.el.css("visibility", "visible").animate({top: top}, 500, function(){
			setTimeout(function(){self.hide();}, 3000);
		});
	},
	hide :function() {
		var self = this;
		this.el.animate({top: -100}, 2000, function(){self.el.css("visibility", "hidden");});
	}
});