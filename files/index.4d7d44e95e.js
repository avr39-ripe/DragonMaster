(function(a){function b(e){if(c[e])return c[e].exports;var f=c[e]={i:e,l:!1,exports:{}};return a[e].call(f.exports,f,f.exports,b),f.l=!0,f.exports}var c={};return b.m=a,b.c=c,b.i=function(e){return e},b.d=function(e,f,g){b.o(e,f)||Object.defineProperty(e,f,{configurable:!1,enumerable:!0,get:g})},b.n=function(e){var f=e&&e.__esModule?function(){return e['default']}:function(){return e};return b.d(f,'a',f),f},b.o=function(e,f){return Object.prototype.hasOwnProperty.call(e,f)},b.p='',b(b.s=5)})([function(a,b,c){'use strict';function f(){Object.keys(o).forEach(function(r){o[r](!0)})}function g(q){if(q.data instanceof ArrayBuffer){var r=new DataView(q.data),s=r.getUint8(m.default.Const.wsCmd),u=r.getUint8(m.default.Const.wsSysId),v=r.getUint8(m.default.Const.wsSubCmd);p.hasOwnProperty(u)&&p[u](r)}}function h(){}function j(){}Object.defineProperty(b,'__esModule',{value:!0}),b.wsBinProcessors=b.wsEnablers=b.websocket=b.initWS=void 0;var l=c(1),m=function(q){return q&&q.__esModule?q:{default:q}}(l),n,o=[],p={};b.initWS=function(){var q='ws://'+location.host+'/ws';b.websocket=n=new WebSocket(q),n.onopen=function(r){f(r)},n.onclose=function(r){h(r)},n.onmessage=function(r){g(r)},n.onerror=function(r){j(r)},n.binaryType='arraybuffer'},b.websocket=n,b.wsEnablers=o,b.wsBinProcessors=p},function(a,b){'use strict';Object.defineProperty(b,'__esModule',{value:!0});var e={Const:{wsCmd:0,wsSysId:1,wsSubCmd:2,wsPayLoadStart:3,wsPayLoadStartGetSetArg:4,wsGetSetArg:3,reservedCmd:0,getCmd:1,setCmd:2,getResponse:3,setAck:4,setNack:5,scAppSetTime:1,scAppGetStatus:2,scAppConfigGet:3,scAppConfigSet:4,scBinStateGetName:1,scBinStateGetState:2,scBinStateSetState:3,scBinStatesGetAll:10,scBinStatesGetAllStates:20,scBinStatesGetAllButtons:30,uidHttpState:0,uidHttpButton:127},Cmd:{Get:function(g,h,j){var k=new ArrayBuffer(3),l=new DataView(k);l.setUint8(e.Const.wsCmd,e.Const.getCmd),l.setUint8(e.Const.wsSysId,h),l.setUint8(e.Const.wsSubCmd,j),g.send(l.buffer)},SetArg:function(g,h,j,k,l){var m=new ArrayBuffer(5),n=new DataView(m);n.setUint8(e.Const.wsCmd,e.Const.setCmd),n.setUint8(e.Const.wsSysId,h),n.setUint8(e.Const.wsSubCmd,j),n.setUint8(e.Const.wsGetSetArg,k),n.setUint8(e.Const.wsPayLoadStartGetSetArg,l),g.send(n.buffer)}}};b.default=e},function(a,b,c){'use strict';function f(){this._counter=0,this._timestamp=0,this._dateStr='',this._timer=0,this._enable=!1}Object.defineProperty(b,'__esModule',{value:!0}),b.default=f;var g=c(0),h=c(1),j=function(k){return k&&k.__esModule?k:{default:k}}(h);f.sysId=1,f.prototype.wsGetAppStatus=function(){j.default.Cmd.Get(g.websocket,1,j.default.Const.scAppGetStatus)},f.prototype.wsBinProcess=function(k){var l=k.getUint8(j.default.Const.wsSubCmd);if(l==j.default.Const.scAppGetStatus){this._counter=k.getUint32(j.default.Const.wsPayLoadStart,!0),this._timestamp=k.getUint32(j.default.Const.wsPayLoadStart+4,!0);var m=new Date;m.setTime(1e3*this._timestamp),this._dateStr=m.toLocaleString().replace(/,\ /,'<br>'),this.renderStatus()}},f.prototype.render=function(){var k=document.querySelector('#AppStatusClass'),l=document.importNode(k.content,!0),m=document.getElementById('Container-AppStatusClass');m.appendChild(l)},f.prototype.renderStatus=function(){document.querySelector('#AppStatusClassCounter').textContent=this._counter,document.querySelector('#AppStatusClassDateTime').innerHTML=this._dateStr},f.prototype.remove=function(){var k=document.querySelector('#Container-AppStatusClass');this.removeChilds(k)},f.prototype.removeChilds=function(k){for(var l;l=k.lastChild;)k.removeChild(l)},f.prototype.enable=function(k){k!=this._enable&&(this._enable=k,this._enable?(this.render(),this.wsGetAppStatus(),this._timer=setInterval(this.wsGetAppStatus,5e3)):(clearInterval(this._timer),this.remove()))}},function(a,b,c){'use strict';function f(l){this.uid=l,this._state=0,this._name='',this.render()}function g(){this._binStatesHttp={},this._statesEnable=!1,this._buttonsEnable=!1,this._enable=!1}Object.defineProperty(b,'__esModule',{value:!0}),b.default=g;var h=c(0),j=c(1),k=function(l){return l&&l.__esModule?l:{default:l}}(j);f.sysId=2,f.prototype.wsGet=function(l){var m=new ArrayBuffer(3),n=new DataView(m);n.setUint8(k.default.Const.wsCmd,k.default.Const.getCmd),n.setUint8(k.default.Const.wsSysId,f.sysId),n.setUint8(k.default.Const.wsSubCmd,l),h.websocket.send(n.buffer)},f.prototype.wsGetName=function(){this.wsGet(1)},f.prototype.wsGetState=function(){this.wsGet(2)},f.prototype.wsSetState=function(l){k.default.Cmd.SetArg(h.websocket,f.sysId,k.default.Const.scBinStateSetState,this.uid,l)},f.prototype.wsGotName=function(l){var m=new Uint8Array(l.byteLength-(k.default.Const.wsPayLoadStart+1));console.log.bind(console)('uid = '+this.uid+', bin.byteLength = '+l.byteLength);for(var n=0;n<m.length;n++)m[n]=l.getUint8(k.default.Const.wsPayLoadStart+1+n);this._name=new TextDecoder().decode(m),this.renderName()},f.prototype.wsGotState=function(l){this._state=l.getUint8(k.default.Const.wsPayLoadStart+1,!0),this.renderState()},f.prototype.render=function(){if(this.isState()){var l=document.querySelector('#BinStateHttpClass'),m=document.importNode(l.content,!0);m.querySelector('#binStateDiv').id='binStateDiv'+this.uid,m.querySelector('#binStatePanel').id='binStatePanel'+this.uid,m.querySelector('#binState').id='binState'+this.uid;var n=document.getElementById('Container-BinStateHttpClassStates')}if(this.isButton()){var l=document.querySelector('#BinStateHttpClassButton'),m=document.importNode(l.content,!0);m.querySelector('#binStateButtonDiv').id='binStateButtonDiv'+this.uid,m.querySelector('#binStateButton').addEventListener('mousedown',this),m.querySelector('#binStateButton').addEventListener('mouseup',this),m.querySelector('#binStateButton').id='binStateButton'+this.uid;var n=document.getElementById('Container-BinStateHttpClassButtons')}n.appendChild(m)},f.prototype.renderName=function(){this.isState()&&(document.querySelector('#binState'+this.uid).textContent=this._name),this.isButton()&&(document.querySelector('#binStateButton'+this.uid).textContent=this._name)},f.prototype.renderState=function(){if(this.isState()){var l=document.querySelector('#binStatePanel'+this.uid);this._state?(l.classList.remove('panel-primary'),l.classList.add('panel-danger')):(l.classList.remove('panel-danger'),l.classList.add('panel-primary'))}if(this.isButton()){var l=document.querySelector('#binStateButton'+this.uid);this._state?(l.classList.remove('btn-primary'),l.classList.add('btn-warning')):(l.classList.remove('btn-warning'),l.classList.add('btn-primary'))}},f.prototype.remove=function(){var l=this.isState()?'#binStateDiv'+this.uid:'#binStateButtonDiv'+this.uid,m=document.querySelector(l);this.removeChilds(m),m.remove()},f.prototype.removeChilds=function(l){for(var m;m=l.lastChild;)l.removeChild(m)},f.prototype.wsBinProcess=function(l){var m=l.getUint8(k.default.Const.wsSubCmd);m==k.default.Const.scBinStateGetName&&this.wsGotName(l),m==k.default.Const.scBinStateGetState&&this.wsGotState(l)},f.prototype.isButton=function(){return this.uid>=k.default.Const.uidHttpButton},f.prototype.isState=function(){return this.uid<k.default.Const.uidHttpButton},f.prototype.handleEvent=function(l){switch(l.type){case'mousedown':this.wsSetState(1);break;case'mouseup':this.wsSetState(0);}},g.sysId=2,g.prototype.enableStates=function(l){if(l!=this._statesEnable)if(this._statesEnable=l,!this._statesEnable){var m=this;Object.keys(this._binStatesHttp).forEach(function(n){m.isState(n)&&(m._binStatesHttp[n].remove(),delete m._binStatesHttp[n])})}else this.wsGetAllStates()},g.prototype.enableButtons=function(l){if(l!=this._buttonsEnable)if(this._buttonsEnable=l,!this._buttonsEnable){var m=this;Object.keys(this._binStatesHttp).forEach(function(n){m.isButton(n)&&(m._binStatesHttp[n].remove(),delete m._binStatesHttp[n])})}else this.wsGetAllButtons()},g.prototype.enable=function(l){l!=this._enable&&(this._enable=l,this.enableStates(this._enable),this.enableButtons(this._enable))},g.prototype.wsGetAll=function(){k.default.Cmd.Get(h.websocket,g.sysId,k.default.Const.scBinStatesGetAll)},g.prototype.wsGetAllStates=function(){k.default.Cmd.Get(h.websocket,g.sysId,k.default.Const.scBinStatesGetAllStates)},g.prototype.wsGetAllButtons=function(){k.default.Cmd.Get(h.websocket,g.sysId,k.default.Const.scBinStatesGetAllButtons)},g.prototype.wsBinProcess=function(l){var m=l.getUint8(k.default.Const.wsSubCmd),n=l.getUint8(k.default.Const.wsPayLoadStart);(this.isState(n)&&this._statesEnable||this.isButton(n)&&this._buttonsEnable)&&(m==k.default.Const.scBinStateGetName&&(!this._binStatesHttp.hasOwnProperty(n)&&(this._binStatesHttp[n]=new f(n)),this._binStatesHttp[n].wsGotName(l)),m==k.default.Const.scBinStateGetState&&this._binStatesHttp[n].wsGotState(l))},g.prototype.isButton=function(l){return l>=k.default.Const.uidHttpButton},g.prototype.isState=function(l){return l<k.default.Const.uidHttpButton}},function(a,b){'use strict';function e(h){return fetch(h).then(function(j){if(200<=j.status&&300>j.status)return j.json()})}function f(h,j){this._id=j,this.uid=h,this._temperature=0,this._statusFlag=0,this._name=h,this.render()}function g(h,j){this._id=j,this._url=h,this._tempsensorsHttp={},this._tempsensorsEnable=!1}Object.defineProperty(b,'__esModule',{value:!0}),b.default=g;f.statusFlags={INVALID:1,DISCONNECTED:2},f.prototype.wsGotName=function(h){this._name=h,this.renderName()},f.prototype.wsGotTemperature=function(h,j){this._temperature=h,this._statusFlag=j,this.renderTemperature()},f.prototype.render=function(){var h=document.querySelector('#TempsensorClass'),j=document.importNode(h.content,!0);j.querySelector('#TempsensorClassDiv').id='TempsensorClassDiv'+(this._id+this.uid),j.querySelector('#TempsensorClassPanel').id='TempsensorClassPanel'+(this._id+this.uid),j.querySelector('#TempsensorClassName').id='TempsensorClassName'+(this._id+this.uid),j.querySelector('#TempsensorClassTemperature').id='TempsensorClassTemperature'+(this._id+this.uid);var k=document.getElementById('Container-TempsensorClassTemperatures');k.appendChild(j)},f.prototype.renderName=function(){document.querySelector('#TempsensorClassName'+(this._id+this.uid)).textContent='Temperature #'+this._name},f.prototype.renderTemperature=function(){var h=document.querySelector('#TempsensorClassPanel'+(this._id+this.uid));0==this._statusFlag?(h.classList.remove('panel-danger'),h.classList.remove('panel-warning'),h.classList.add('panel-default')):this._statusFlag&f.statusFlags.DISCONNECTED?(h.classList.remove('panel-default'),h.classList.remove('panel-warning'),h.classList.add('panel-danger')):this._statusFlag&f.statusFlags.INVALID&&(h.classList.remove('panel-default'),h.classList.remove('panel-danger'),h.classList.add('panel-warning')),document.querySelector('#TempsensorClassTemperature'+(this._id+this.uid)).textContent=this._temperature+' \xB0C'},f.prototype.remove=function(){var h=document.querySelector('TempsensorClassDiv'+(this._id+this.uid));this.removeChilds(h),h.remove()},f.prototype.removeChilds=function(h){for(var j;j=h.lastChild;)h.removeChild(j)},g.prototype.enable=function(h){if(h!=this._tempsensorsEnable)if(this._tempsensorsEnable=h,!this._tempsensorsEnable){var j=this;Object.keys(this._tempsensorsHttp).forEach(function(k){j._tempsensorsHttp[k].remove(),delete j._tempsensorsHttp[k]})}else this.wsGetAllTemperatures()},g.prototype.wsGetAllTemperatures=function(){var h=this;e(this._url).then(function(j){Object.keys(j).forEach(function(k){h._tempsensorsHttp.hasOwnProperty(k)||(h._tempsensorsHttp[k]=new f(k,h._id)),h._tempsensorsHttp[k].wsGotName(k),h._tempsensorsHttp[k].wsGotTemperature(j[k].temperature,j[k].statusFlag)})})}},function(a,b,c){'use strict';function e(o){return o&&o.__esModule?o:{default:o}}var g=c(2),h=e(g),j=c(3),k=e(j),l=c(0),m=c(4),n=e(m);document.addEventListener('DOMContentLoaded',function(){var o=new h.default,p=new k.default;l.wsEnablers.push(o.enable.bind(o)),l.wsEnablers.push(p.enableStates.bind(p)),l.wsEnablers.push(p.enableButtons.bind(p)),l.wsBinProcessors[h.default.sysId]=o.wsBinProcess.bind(o),l.wsBinProcessors[k.default.sysId]=p.wsBinProcess.bind(p);var q=new n.default('/temperature.json',0);q.enable(!0),setInterval(function(){q.wsGetAllTemperatures()},5e3);var r=new n.default('/temperatureHome.json',1);r.enable(!0),setInterval(function(){r.wsGetAllTemperatures()},5e3),(0,l.initWS)()})}]);
//# sourceMappingURL=index.4d7d44e95e.js.map