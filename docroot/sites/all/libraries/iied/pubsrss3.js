/*
 *Pull in RSS feed for author and display it
 */

try { google.load("feeds", "1");

  function feedLoaded(result) {
    if (!result.error && result.feed.entries.length > 0) {
      var container = document.getElementById("pubs");
      container.innerHTML = '<h3>Recent publications</h3>';
      for (var i = 0; i < result.feed.entries.length; i++) {
          var entry = result.feed.entries[i];
          var div = document.createElement("div");
          var a = document.createElement("a");
          var img = document.createElement("img");
          var span = document.createElement("span");
          var parts = entry.link.split('/');
          var lastsegment = parts.pop() || parts.pop();
          var imgurl = "http://pubs.iied.org/cover_s/" + lastsegment + ".jpg";
          img.setAttribute("src", imgurl);
          a.appendChild(img);
          a.appendChild(document.createTextNode(htmlDecode(htmlDecode(entry.title))));
          span.appendChild(document.createTextNode(entry.contentSnippet));
          a.appendChild(span);
          a.setAttribute("href",entry.link);
          div.appendChild(a);
          div.setAttribute("class","pubitem");
          container.appendChild(div);
        }
        var diva = document.createElement("div");
        var aa = document.createElement("a");
        aa.setAttribute("href",encodeURI("http://pubs.iied.org/search?a=" + document.getElementById('page-title').innerHTML));
        diva.appendChild(aa);
        diva.setAttribute("class","more-link");
        aa.innerHTML = 'More publications';
        container.appendChild(diva);
      }
    }

  function OnLoad() {
    var feed = new google.feeds.Feed(encodeURI("http://pubs.iied.org/search?f=rss&a=" + document.getElementById('page-title').innerHTML), {
      api_key : 'mfbdkjoqsivdaton59jvrdqleqyy1hzxnrzv7ff8',
      count : 6
    });
    feed.load(feedLoaded);
    }
    google.setOnLoadCallback(OnLoad);
  }
  catch(err) {}

  function htmlDecode(input){
    var e = document.createElement('div');
    e.innerHTML = input;
    return e.childNodes.length === 0 ? "" : e.childNodes[0].nodeValue;
  }
