using System;
using System.Xml;
using System.Net;
using System.IO;
using System.Text;
using System.Web;
using System.Collections.Generic;
public class SoapClient {
    private static string CallWebService(string xml)
    {
        var _url = "http://test.linkcare.es/ws/ServerWSDL.php";
        var _action = "http://linkcare.es/LINKCARE";


        XmlDocument soapEnvelopeXml = CreateSoapEnvelopeWithXML(xml);
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

        return soapResult;

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

    private static XmlDocument CreateSoapEnvelopeWithXML(string xml)
    {
        XmlDocument soapEnvelop = new XmlDocument();
        soapEnvelop.LoadXml(
@"<soapenv:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soapenv=""http://schemas.xmlsoap.org/soap/envelope/"" xmlns:lin=""http://linkcare.es/LINKCARE"">
   <soapenv:Header/>
   <soapenv:Body>
      "+xml+@"
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

    #region - WebServices Calls

    public static string CallSessionDiscover()
    {
        string xml = @"<lin:session_init soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
         <user xsi:type=""xsd:string"">linkcare</user>
         <password xsi:type=""xsd:string"">linkcare</password>
         <IP xsi:type=""xsd:string""></IP>
         <host xsi:type=""xsd:string""></host>
         <language xsi:type=""xsd:string""></language>
         <current_date xsi:type=""xsd:string""></current_date>
         <calendar xsi:type=""xsd:string""></calendar>
         <device xsi:type=""xsd:string""></device>
      </lin:session_init>";
        string soapResult = CallWebService(xml);
        string token = ParseToken(soapResult);
        return token;
    }

    public static Dictionary<string, string>[] CallTaskListOverdue(string token)
    {
        string xml = @"<lin:task_list_overdue soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
         <session xsi:type=""xsd:string"">" + token + @"</session>
         <max_res xsi:type=""xsd:string"">10</max_res>
         <offset xsi:type=""xsd:string"">0</offset>
      </lin:task_list_overdue>";
        string soapResult = CallWebService(xml);
        Dictionary<string, string>[] admissionIds = ParseAdmissionIds(soapResult);
        return admissionIds;
    }

    public static void CallTaskClose(string task_id, string token)
    {
        string xml = @"<lin:task_close soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
         <session xsi:type=""xsd:string"">" + token + @"</session>
         <task_id xsi:type=""xsd:string"">" + task_id + @"</task_id>
      </lin:task_close>";
        string soapResult = CallWebService(xml);
    }

    private static string ParseToken(string xmlString)
    {
        string token = null;

        // Create an XmlReader
        using (XmlReader reader = XmlReader.Create(new StringReader(xmlString)))
        {
            //reader.ReadToFollowing("book");
            //reader.MoveToFirstAttribute();
            //string genre = reader.Value;
            //output.AppendLine("The genre value: " + genre);

            reader.ReadToFollowing("token");
            token = reader.ReadElementContentAsString();
        }
        return token;
    }

    private static Dictionary<string, string>[] ParseAdmissionIds(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        XmlNode ResultUnique = result[0];

        xmlString = HttpUtility.HtmlDecode(ResultUnique.InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList resultTasks = doc.GetElementsByTagName("task");

        //string[] AdmissionsArray = new string[resultAdmissions.Count];
        Dictionary<string, string>[] admissions = new Dictionary<string, string>[resultTasks.Count];

        for (int i = 0; i < resultTasks.Count; i++)
        {
            XmlNodeList task_id = resultTasks[i].SelectNodes("ref");
            XmlNodeList data = resultTasks[i].SelectNodes("data");
            XmlNodeList admission = data[0].SelectNodes("admission");
            XmlNodeList admission_id = admission[0].SelectNodes("ref");
            XmlNodeList admission_case = admission[0].SelectNodes("case");
            XmlNodeList admission_fullname = admission_case[0].SelectNodes("full_name");
            XmlNodeList admission_gender = admission_case[0].SelectNodes("gender");
            XmlNodeList admission_bdate = admission_case[0].SelectNodes("bdate");
            //XmlNodeList admission_fullname = resultAdmissions[i].SelectNodes("full_name");
            //XmlNodeList admission_gender = resultAdmissions[i].SelectNodes("gender");
            //XmlNodeList admission_bdate = resultAdmissions[i].SelectNodes("bdate");
            admissions[i] = new Dictionary<string, string>();
            admissions[i].Add("task_id", task_id[0].FirstChild.Value);
            admissions[i].Add("admission_id", admission_id[0].FirstChild.Value);
            //admissions[i].Add("case", admission_case[0].FirstChild.Value);
            admissions[i].Add("fullname", admission_fullname[0].FirstChild.Value);
            admissions[i].Add("gender", admission_gender[0].FirstChild.Value);
            admissions[i].Add("bdate", admission_bdate[0].FirstChild.Value);
        }

        //Console.WriteLine(AdmissionsArray);
        //ResultUnique.InnerXml = ResultUnique.InnerXml.Replace("&lt;", "<");
        //Console.WriteLine(ResultUnique.InnerXml);

        //XmlNode myNode = ResultUnique.SelectSingleNode("&lt;");
        //myNode.Value = "<";
        //Console.WriteLine(myNode.InnerXml);

        //XmlNodeList TaskList = ResultUnique.GetElementsByTagName("task_list");
        //Console.WriteLine(task_list[0].InnerXml);

        /*
        for (int i = 0; i < result.Count; i++)
        {
            Console.WriteLine(result[i].InnerXml);
        }
        */

        //StringBuilder output = new StringBuilder();
        //string taskId = null;

        // Create an XmlReader
        //using (XmlReader reader = XmlReader.Create(new StringReader(xmlString)))
        //{
            //reader.ReadToFollowing("result");
            //output.AppendLine(reader.ReadElementContentAsString());
            //taskId = output.ToString();
            //Console.Write("hola:" + output);
            /*
            while (reader.Read())
            {
                reader.ReadToFollowing("task");
                reader.MoveToFirstAttribute();
                reader.ReadToFollowing("ref");
                output.AppendLine(reader.ReadElementContentAsString());
                taskId = output.ToString();
                Console.Write("Este es el taskId:" + taskId);
            }
            
            reader.ReadToFollowing("task_list");
            reader.MoveToFirstAttribute();
            reader.ReadToFollowing("task");
            reader.MoveToFirstAttribute();
            //string genre = reader.Value;
            //output.AppendLine("The genre value: " + genre);

            reader.ReadToFollowing("ref");
            output.AppendLine(reader.ReadElementContentAsString());
            taskId = output.ToString();
            Console.Write("Este es el taskId:" + taskId);
            */
        //}
        return admissions;
    }

    #endregion
}
