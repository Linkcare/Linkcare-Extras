using System;
using System.Xml;
using System.Net;
using System.IO;
using System.Text;
public class SoapClient {
    public static void CallWebService()
    {
        var _url = "http://test.linkcare.es/ws/ServerWSDL.php";
        var _action = "http://linkcare.es/LINKCARE";

        XmlDocument soapEnvelopeXml = CreateSoapEnvelope();
        HttpWebRequest webRequest = CreateWebRequest(_url, _action);
        InsertSoapEnvelopeIntoWebRequest(soapEnvelopeXml, webRequest);

        // begin async call to web request.
        IAsyncResult asyncResult = webRequest.BeginGetResponse(null, null);

        // suspend this thread until call is complete. You might want to
        // do something usefull here like update your UI.
        asyncResult.AsyncWaitHandle.WaitOne();

        // get the response from the completed web request.
        string soapResult;
        using (WebResponse webResponse = webRequest.EndGetResponse(asyncResult))
        {
            using (StreamReader rd = new StreamReader(webResponse.GetResponseStream()))
            {
                soapResult = rd.ReadToEnd();
            }
            Console.Write(soapResult);
        }
        ParseToken(soapResult);
    }

    private static HttpWebRequest CreateWebRequest(string url, string action)
    {
        HttpWebRequest webRequest = (HttpWebRequest)WebRequest.Create(url);
        webRequest.Headers.Add("SOAPAction", action);
        webRequest.ContentType = "text/xml;charset=\"utf-8\"";
        webRequest.Accept = "text/xml";
        webRequest.Method = "POST";
        return webRequest;
    }

    private static XmlDocument CreateSoapEnvelope()
    {
        XmlDocument soapEnvelop = new XmlDocument();
        soapEnvelop.LoadXml(
@"<soapenv:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soapenv=""http://schemas.xmlsoap.org/soap/envelope/"" xmlns:lin=""http://linkcare.es/LINKCARE"">
   <soapenv:Header/>
   <soapenv:Body>
      <lin:session_init soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
         <user xsi:type=""xsd:string"">linkcare</user>
         <password xsi:type=""xsd:string"">linkcare</password>
         <IP xsi:type=""xsd:string""></IP>
         <host xsi:type=""xsd:string""></host>
         <language xsi:type=""xsd:string""></language>
         <current_date xsi:type=""xsd:string""></current_date>
         <calendar xsi:type=""xsd:string""></calendar>
         <device xsi:type=""xsd:string""></device>
      </lin:session_init>
   </soapenv:Body>
</soapenv:Envelope>");
        return soapEnvelop;
    }

    private static void InsertSoapEnvelopeIntoWebRequest(XmlDocument soapEnvelopeXml, HttpWebRequest webRequest)
    {
        using (Stream stream = webRequest.GetRequestStream())
        {
            soapEnvelopeXml.Save(stream);
        }
    }

    private static void ParseToken(string xmlString)
    {
        Console.Write("hola");
        StringBuilder output = new StringBuilder();

        // Create an XmlReader
        using (XmlReader reader = XmlReader.Create(new StringReader(xmlString)))
        {
            //reader.ReadToFollowing("book");
            //reader.MoveToFirstAttribute();
            //string genre = reader.Value;
            //output.AppendLine("The genre value: " + genre);

            reader.ReadToFollowing("token");
            output.AppendLine("Content of the token element: " + reader.ReadElementContentAsString());
        }
        Console.Write(output);
    }
}
