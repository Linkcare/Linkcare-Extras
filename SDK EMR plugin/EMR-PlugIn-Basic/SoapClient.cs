using System;
using System.Xml;
using System.Net;
using System.IO;
using System.Text;
using System.Web;
using System.Collections.Generic;
public class SoapClient {

    public static class GlobalVar
    {
        static string _user;
        static string _password;
        static string _url;
        static string _action;
        static string _progcode;
        static string _taskcode;
        static string _taskcodespirometryrequest;

        public static string User
        {
            get
            {
                return _user;
            }
            set
            {
                _user = value;
            }
        }

        public static string Password
        {
            get
            {
                return _password;
            }
            set
            {
                _password = value;
            }
        }

        public static string Url
        {
            get
            {
                return _url;
            }
            set
            {
                _url = value;
            }
        }

        public static string Action
        {
            get
            {
                return _action;
            }
            set
            {
                _action = value;
            }
        }

        public static string Progcode
        {
            get
            {
                return _progcode;
            }
            set
            {
                _progcode = value;
            }
        }

        public static string Taskcode
        {
            get
            {
                return _taskcode;
            }
            set
            {
                _taskcode = value;
            }
        }

        public static string Taskcodespirometryrequest
        {
            get
            {
                return _taskcodespirometryrequest;
            }
            set
            {
                _taskcodespirometryrequest = value;
            }
        }
    }

    public static void readConfigurationFile()
    {
        XmlDocument doc = new XmlDocument();
        doc.Load(@"C:\Users\jcarballo\Desktop\config.xml");
        XmlNodeList data = doc.GetElementsByTagName("data");
        XmlNodeList user = data[0].SelectNodes("user");
        XmlNodeList password = data[0].SelectNodes("password");
        XmlNodeList url = data[0].SelectNodes("url");
        XmlNodeList action = data[0].SelectNodes("action");
        XmlNodeList progcode = data[0].SelectNodes("progcode");
        XmlNodeList taskcode = data[0].SelectNodes("taskcode");
        XmlNodeList taskcodespirometryrequest = data[0].SelectNodes("taskcodespirometryrequest");
        GlobalVar.User = user[0].FirstChild.Value;
        GlobalVar.Password = password[0].FirstChild.Value;
        GlobalVar.Url = url[0].FirstChild.Value;
        GlobalVar.Action = action[0].FirstChild.Value;
        GlobalVar.Progcode = progcode[0].FirstChild.Value;
        GlobalVar.Taskcode = taskcode[0].FirstChild.Value;
        GlobalVar.Taskcodespirometryrequest = taskcodespirometryrequest[0].FirstChild.Value;
    }

    private static string CallWebService(string xml)
    {
        XmlDocument soapEnvelopeXml = CreateSoapEnvelopeWithXML(xml);
        HttpWebRequest webRequest = CreateWebRequest(GlobalVar.Url, GlobalVar.Action);
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
                         <user xsi:type=""xsd:string"">" + GlobalVar.User + @"</user>
                         <password xsi:type=""xsd:string"">" + GlobalVar.Password + @"</password>
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
                         <task xsi:type=""xsd:string"">" + task_id + @"</task>
                      </lin:task_close>";
        string soapResult = CallWebService(xml);
    }

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
                            <admission xsi:type=""xsd:string""></admission>
                       </lin:case_get>";
        string soapResult = CallWebService(xml);
        patient_data = ParseDataPatient(soapResult);
        return patient_data;
    }

    public static string CallAdmissionListCase(string case_id, string token)
    {
        string xml = @"<lin:admission_list_case soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <case xsi:type=""xsd:string"">" + case_id + @"</case>
                      </lin:admission_list_case>";
        string soapResult = CallWebService(xml);
        string admission_id = ParseAdmissionFromCase(soapResult);
        return admission_id;
    }

    public static string CallTaskInsertByTaskCode(string admission_id, string token)
    {
        DateTime now = DateTime.Now;
        string date_now = now.ToString().Replace("/", "-");

        string xml = @"<lin:task_insert_by_task_code soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <admission xsi:type=""xsd:string"">" + admission_id + @"</admission>
                         <task_code xsi:type=""xsd:string"">" + GlobalVar.Taskcode + @"</task_code>
                         <date xsi:type=""xsd:string"">" + date_now + @"</date>
                      </lin:task_insert_by_task_code>";
        string soapResult = CallWebService(xml);
        string task_id = ParseTaskId(soapResult);
        return task_id;
    }

    public static string CallTaskFormList(string task_id, string token)
    {
        string xml = @"<lin:task_form_list soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <task_id xsi:type=""xsd:string"">" + task_id + @"</task_id>
                       </lin:task_form_list>";
        string soapResult = CallWebService(xml);
        string form_id = ParseFormId(soapResult);
        return form_id;
    }

    public static string CallFormGetSummary(string form_id, string token)
    {
        string xml = @"<lin:form_get_summary soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <form xsi:type=""xsd:string"">" + form_id + @"</form>
                         <with_questions xsi:type=""xsd:string""></with_questions>
                      </lin:form_get_summary>";
        string soapResult = CallWebService(xml);
        string answer_id = ParseAnswerId(soapResult);
        return answer_id;
    }

    public static void CallFormSetAnswer(string token, string form_id, string answer_id, string admission_id, string xml_to_save)
    {
        var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(xml_to_save);
        string value = System.Convert.ToBase64String(plainTextBytes);
        string xml = @"<lin:form_set_answer soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <form_id xsi:type=""xsd:string"">" + form_id + @"</form_id>
                         <question_id xsi:type=""xsd:string"">" + answer_id + @"</question_id>
                         <value xsi:type=""xsd:string"">" + admission_id + @"_ndd.xml|" + value + @"</value>
                         <option_id xsi:type=""xsd:string""></option_id>
                         <event_id xsi:type=""xsd:string""></event_id>
                         <group_close xsi:type=""xsd:string""></group_close>
                         <formula_calculate xsi:type=""xsd:string""></formula_calculate>
                         <get_next_question xsi:type=""xsd:string""></get_next_question> 0
                      </lin:form_set_answer>";
        string soapResult = CallWebService(xml);
        //string form_id = ParseFormId(soapResult);
        //return form_id;
    }

    private static string ParseToken(string xmlString)
    {
        string token = null;

        // Create an XmlReader
        using (XmlReader reader = XmlReader.Create(new StringReader(xmlString)))
        {
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
        
        Dictionary<string, string>[] admissions = new Dictionary<string, string>[resultTasks.Count];

        int j = 0;
        for (int i = 0; i < resultTasks.Count; i++)
        {
            XmlNodeList task_id = resultTasks[i].SelectNodes("ref");
            XmlNodeList data = resultTasks[i].SelectNodes("data");
            XmlNodeList task_code = data[0].SelectNodes("task_code");
            XmlNodeList admission = data[0].SelectNodes("admission");
            XmlNodeList admission_id = admission[0].SelectNodes("ref");
            XmlNodeList admission_case = admission[0].SelectNodes("case");
            XmlNodeList admission_name = admission_case[0].SelectNodes("name");
            XmlNodeList admission_surname = admission_case[0].SelectNodes("surname");
            XmlNodeList admission_gender = admission_case[0].SelectNodes("gender");
            XmlNodeList admission_bdate = admission_case[0].SelectNodes("bdate");
            if (task_code[0].InnerText.Equals(GlobalVar.Taskcodespirometryrequest))
            {
                admissions[j] = new Dictionary<string, string>();
                admissions[j].Add("task_id", task_id[0].FirstChild.Value);
                admissions[j].Add("admission_id", admission_id[0].FirstChild.Value);
                admissions[j].Add("name", admission_name[0].FirstChild.Value);
                admissions[j].Add("surname", (admission_surname[0].InnerText.Equals("")) ? "" : admission_surname[0].FirstChild.Value);
                admissions[j].Add("gender", admission_gender[0].FirstChild.Value);
                admissions[j].Add("bdate", admission_bdate[0].FirstChild.Value);
                j++;
            }
        }
        
        return admissions;
    }

    private static string ParseAdmissionFromCase(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList admissions = doc.SelectNodes("admissions");
        XmlNodeList admission = admissions[0].SelectNodes("admission");
        string admission_id = "";
        for (int i = 0; i < admission.Count; i++)
        {
            XmlNodeList admission_ref = admission[i].SelectNodes("ref");
            XmlNodeList data = admission[i].SelectNodes("data");
            XmlNodeList subscription = data[0].SelectNodes("subscription");
            XmlNodeList program = subscription[0].SelectNodes("program");
            XmlNodeList prog_code = program[0].SelectNodes("prog_code");
            if (prog_code[0].FirstChild.Value.Equals(GlobalVar.Progcode))
            {
                admission_id = admission_ref[0].FirstChild.Value;
            }
        }
        return admission_id;
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
        XmlNodeList name = data[0].SelectNodes("name");
        XmlNodeList surname = data[0].SelectNodes("surname");
        XmlNodeList nickname = data[0].SelectNodes("nickname");
        XmlNodeList bdate = data[0].SelectNodes("bdate");
        XmlNodeList gender = data[0].SelectNodes("gender");

        Dictionary<string, string> data_patient = new Dictionary<string, string>();
        data_patient.Add("name", name[0].FirstChild.Value);
        data_patient.Add("surname", (surname[0].InnerText.Equals("")) ? "" : surname[0].FirstChild.Value);
        data_patient.Add("nickname", nickname[0].FirstChild.Value);
        data_patient.Add("bdate", bdate[0].FirstChild.Value);
        data_patient.Add("gender", gender[0].FirstChild.Value);

        return data_patient;
    }
    
    private static string ParseTaskId(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        return result[0].FirstChild.Value;
    }
    
    private static string ParseFormId(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList forms = doc.SelectNodes("forms");
        XmlNodeList form = forms[0].SelectNodes("form");
        string spiro2_form_id = "";
        for (int i=0; i< form.Count; i++)
        {
            XmlNodeList form_code = form[i].SelectNodes("form_code");
            if (form_code[0].FirstChild.Value.Equals("SPIROS2"))
            {
                XmlNodeList form_id = form[i].SelectNodes("ref");
                spiro2_form_id = form_id[0].FirstChild.Value;
            }
        }
        return spiro2_form_id;
    }

    private static string ParseAnswerId(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList form = doc.SelectNodes("form");
        XmlNodeList data = form[0].SelectNodes("data");
        XmlNodeList questions = data[0].SelectNodes("questions");
        XmlNodeList question = questions[0].SelectNodes("question");
        string answer_id = "";
        for (int i = 0; i < question.Count; i++)
        {
            XmlNodeList type = question[i].SelectNodes("type");
            if (type[0].FirstChild.Value.Equals("FILE"))
            {
                XmlNodeList question_id = question[i].SelectNodes("question_id");
                answer_id = question_id[0].FirstChild.Value;
            }
        }
        return answer_id;
    }

    #endregion
}
