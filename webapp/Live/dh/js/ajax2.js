// The AJAX function...

function AJAX(){
try{
xmlHttp=new XMLHttpRequest(); // Firefox, Opera 8.0+, Safari
return xmlHttp;
}
catch (e){
try{
xmlHttp=new ActiveXObject("Msxml2.XMLHTTP"); // Internet Explorer
return xmlHttp;
}
catch (e){
try{
xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
return xmlHttp;
}
catch (e){
alert("Your browser does not support AJAX.");
return false;
}
}
}
}

// Timestamp for preventing IE caching the GET request (common function)

function fetch_unix_timestamp()
{
 return parseInt(new Date().getTime().toString().substring(0, 10))
}

////////////////////////////////
//
// Refreshing the DIV TIMEDIV
//
////////////////////////////////

function refreshdiv_wrapper(){

// Customise those settings

// Create xmlHttp

var xmlHttp_wrapper = AJAX();

// No cache

var timestamp_wrapper = fetch_unix_timestamp();
var nocacheurl_wrapper = url_wrapper+"?t="+timestamp_wrapper;
nocacheurl_wrapper = url_wrapper;
// The code...

xmlHttp_wrapper.onreadystatechange=function(){
if(xmlHttp_wrapper.readyState==4){
document.getElementById(divid_wrapper).innerHTML=xmlHttp_wrapper.responseText;
setTimeout('refreshdiv_wrapper()',seconds_wrapper*1000);
}
}
xmlHttp_wrapper.open("GET",nocacheurl_wrapper,true);
xmlHttp_wrapper.send(null);
}

// Start the refreshing process

window.onload = function startrefresh(){
setTimeout('refreshdiv_wrapper()',seconds_wrapper*1000);
}

function refreshdiv_leftpanel(){

// Customise those settings

// Create xmlHttp

var xmlHttp_leftpanel = AJAX();

// No cache

var timestamp_leftpanel = fetch_unix_timestamp();
var nocacheurl_leftpanel = url_leftpanel+"?t="+timestamp_leftpanel;

nocacheurl_leftpanel = url_leftpanel;

// The code...

xmlHttp_leftpanel.onreadystatechange=function(){
if(xmlHttp_leftpanel.readyState==4){
document.getElementById(divid_leftpanel).innerHTML=xmlHttp_leftpanel.responseText;
setTimeout('refreshdiv_leftpanel()',seconds_leftpanel*1000);
}
}
xmlHttp_leftpanel.open("GET",nocacheurl_leftpanel,true);
xmlHttp_leftpanel.send(null);
}

// Start the refreshing process

window.onload = function startrefresh(){
setTimeout('refreshdiv_leftpanel()',seconds_leftpanel*1000);
}



