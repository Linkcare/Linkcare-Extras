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

    /*
    public static void CallTaskClose(string task_id, string token)
    {
        string xml = @"<lin:task_close soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
         <session xsi:type=""xsd:string"">" + token + @"</session>
         <task_id xsi:type=""xsd:string"">" + task_id + @"</task_id>
      </lin:task_close>";
        string soapResult = CallWebService(xml);
    }
    */

    public static string CallCaseInsert(string cip, string token)
    {
        string data = @"<?xml version=""1.0"" encoding=""UTF-8""?><case><ref></ref><given_name></given_name><family_name></family_name><refs><ref>NICK/</ref><ref>NIF/</ref><ref>NIE/</ref><ref>PAS/</ref><ref>CIP/" + cip + @"</ref></refs ><data><bdate></bdate><age></age><gender></gender><status></status><preferences><password_expire></password_expire></preferences></data></case>";

        string cdata = HttpUtility.HtmlEncode(data);

        string xml = @"<lin:case_insert soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                            <session xsi:type=""xsd:string"">" + token + @"</session>
                            <case xsi:type=""xsd:string"">" + cdata + @"</case>
                       </lin:case_insert>";
        string soapResult = CallWebService(xml);
        string case_id = ParseCaseId(soapResult);
        return case_id;
    }

    public static Dictionary<string, string> CallCaseGet(string case_id, string token)
    {
        Dictionary<string, string> patient_data = new Dictionary<string, string>();

        string xml = @"<lin:case_get soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                            <session xsi:type=""xsd:string"">" + token + @"</session>
                            <case xsi:type=""xsd:string"">" + case_id + @"</case>
                            <admission xsi:type=""xsd:string"">?</admission>
                       </lin:case_get>";
        string soapResult = CallWebService(xml);
        patient_data = ParseDataPatient(soapResult);
        return patient_data;
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

    private static string ParseCaseId(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        XmlNode ResultUnique = result[0];
        XmlNodeList type = doc.GetElementsByTagName("type");
        XmlNode TypeUnique = type[0];
        string type_string = TypeUnique.InnerText;
        string result_string = null;

        if (type_string.Equals("EXIST"))
        {
            result_string = ResultUnique.InnerText;
        }

        return result_string;
    }

    private static Dictionary<string, string> ParseDataPatient(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");

        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);

        XmlNodeList var_case = doc.SelectNodes("case");
        XmlNodeList data = var_case[0].SelectNodes("data");
        XmlNodeList fullname = data[0].SelectNodes("full_name");
        XmlNodeList nickname = data[0].SelectNodes("nickname");
        XmlNodeList bdate = data[0].SelectNodes("bdate");
        XmlNodeList gender = data[0].SelectNodes("gender");

        Dictionary<string, string> data_patient = new Dictionary<string, string>();
        data_patient.Add("fullname", fullname[0].FirstChild.Value);
        data_patient.Add("nickname", nickname[0].FirstChild.Value);
        data_patient.Add("bdate", bdate[0].FirstChild.Value);
        data_patient.Add("gender", gender[0].FirstChild.Value);

        /*
        xmlString = HttpUtility.HtmlDecode(ResultUnique.InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList resultTasks = doc.GetElementsByTagName("task");

        //string[] AdmissionsArray = new string[resultAdmissions.Count];
        

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
            */
        return data_patient;
    }

    #endregion
}
