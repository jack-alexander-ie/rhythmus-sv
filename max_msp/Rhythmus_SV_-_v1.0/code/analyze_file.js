var access_token = 0;
var current = 0;
var ajaxreq;

function token(val) 
{
	post("token set to", val, "\n");
	access_token = val;
}

function set_current(val) 
{
	current = val;
}

//function analyze(id)
//{
//	var url = "http://www.freesound.org/apiv2/sounds/" + id + "/analysis/?descriptors=rhythm.bpm";	
//	ajaxreq = new XMLHttpRequest();
//	ajaxreq.open("GET", url);
//	ajaxreq.setRequestHeader("Authorization", "Bearer " + access_token);
//	ajaxreq.onreadystatechange = readystatechange;
//	ajaxreq.send();
//}

function analyze(id)
{
	
	var url = "http://www.freesound.org/apiv2/sounds/" + id + "/analysis/?descriptors=rhythm.bpm";
	
    try
    {
        ajaxreq = new XMLHttpRequest();
    }
    catch (e)
    {
        ajaxreq = new ActiveXObject('Microsoft.XMLHTTP');
    }

    ajaxreq.open("GET", url);
    ajaxreq.setRequestHeader("Authorization", "Bearer " + access_token);
	ajaxreq.onreadystatechange = readystatechange;
	ajaxreq.send();
}



function readystatechange()
{
	if (this.readyState == 4) 
	{
		var result_json = JSON.parse(this.responseText);
		
		//post(this.responseText);
		
		if (result_json && result_json.rhythm && result_json.rhythm.bpm) 
		{
			outlet(0, "tempo", result_json.rhythm.bpm);
		}
	}
}