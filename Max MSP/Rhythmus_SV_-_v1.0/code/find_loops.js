outlets=2;

var ajaxreq;
var more = "";
var clientsecret;
//var url = "http://www.freesound.org/apiv2/search/text/?token=XXXX&filter=tag:(loop AND house AND beat) type:aif channels:2";
var url = "";
url = encodeURI(url);

function seturl(val) 
{
	url = encodeURI(val)
}

function set_clientsecret(val)
{
	clientsecret = val;
}

//function findloops()
//{
//	ajaxreq = new XMLHttpRequest();
//	ajaxreq.open("GET", url);
//	ajaxreq.onreadystatechange = readystatechange_parsejson;
//	ajaxreq.send();
//}

function findloops()
{
    try
    {
        ajaxreq = new XMLHttpRequest();
    }
    catch (e)
    {
        ajaxreq = new ActiveXObject('Microsoft.XMLHTTP');
    }

    ajaxreq.open("GET", url);
    ajaxreq.onreadystatechange = readystatechange_parsejson;
    ajaxreq.send();
}

function readystatechange_parsejson()
{
	if (this.readyState == 4)
	{
		var id_list = "";
 		var myjson = JSON.parse(this.responseText);
		var do_more = Math.floor((Math.random() * 2));

		post("domore: ", do_more, "\n");
		more = myjson['next'];

		if (do_more == 1 && more) 
		{
			post("looking for more\n");
			findmore();
		} 
		else 
		{
			outlet(1, this.responseText);

			if (myjson && myjson.results) 
			{ 
        		for (var i=0; i< myjson.results.length; i++) 
				{
					id_list = id_list + myjson.results[i].id + " ";
				}
			
				outlet(0, id_list);
			}
		}
	}
}

function findmore()
{
	post("more: ", more, "\n");
	more += "&token=";
	more += clientsecret;
	ajaxreq.open("GET", more);
	ajaxreq.send();
}
