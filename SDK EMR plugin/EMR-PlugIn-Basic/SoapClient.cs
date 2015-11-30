using System;
using System.Xml;
using System.Net;
using System.IO;
using System.Text;
using System.Linq;
using System.Web;
using System.Collections.Generic;
using System.Windows.Forms;
using Newtonsoft.Json;

public class SoapClient
{

    public static class GlobalVar
    {
        static string _user;
        static string _password;
        static string _url;
        static string _role;
        static string _team;
        static string _progcode;
        static string _taskcode;
        static string _taskcodespirometryrequest;
        static string _form_code;
        static string _id;
        static string _logs;
        static string _config;
        static string _patientsFile;

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

        public static string Role
        {
            get
            {
                return _role;
            }
            set
            {
                _role = value;
            }
        }

        public static string Team
        {
            get
            {
                return _team;
            }
            set
            {
                _team = value;
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

        public static string Id
        {
            get
            {
                return _id;
            }
            set
            {
                _id = value;
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

        public static string Formcode
        {
            get
            {
                return _form_code;
            }
            set
            {
                _form_code = value;
            }
        }

        public static string Logs
        {
            get
            {
                return _logs;
            }
            set
            {
                _logs = value;
            }
        }

        public static string Config
        {
            get
            {
                return _config;
            }
            set
            {
                _config = value;
            }
        }

        public static string PatientsFile
        {
            get
            {
                return _patientsFile;
            }
            set
            {
                _patientsFile = value;
            }
        }
    }

    public static void readConfigurationFile()
    {
        var dir = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;
        GlobalVar.Logs = dir + @"\errors.log";
        GlobalVar.Config = dir + @"\config.xml";
        GlobalVar.PatientsFile = dir + @"\patients.xml";
        MakeConfigFile();
        XmlDocument doc = new XmlDocument();
        doc.Load(GlobalVar.Config);
        XmlNodeList data = doc.GetElementsByTagName("data");
        XmlNodeList user = data[0].SelectNodes("user");
        XmlNodeList password = data[0].SelectNodes("password");
        XmlNodeList url = data[0].SelectNodes("url");
        XmlNodeList action = data[0].SelectNodes("action");
        XmlNodeList role = data[0].SelectNodes("role");
        XmlNodeList team = data[0].SelectNodes("team");
        XmlNodeList progcode = data[0].SelectNodes("progcode");
        XmlNodeList taskcode = data[0].SelectNodes("taskcode");
        XmlNodeList taskcodespirometryrequest = data[0].SelectNodes("taskcodespirometryrequest");
        XmlNodeList formcode = data[0].SelectNodes("formcode");
        XmlNodeList id = data[0].SelectNodes("id");
        GlobalVar.User = user[0].FirstChild.Value;
        GlobalVar.Password = password[0].FirstChild.Value;
        GlobalVar.Url = url[0].FirstChild.Value;
        GlobalVar.Role = role[0].FirstChild.Value;
        GlobalVar.Team = team[0].FirstChild.Value;
        GlobalVar.Progcode = progcode[0].FirstChild.Value;
        GlobalVar.Taskcode = taskcode[0].FirstChild.Value;
        GlobalVar.Taskcodespirometryrequest = taskcodespirometryrequest[0].FirstChild.Value;
        GlobalVar.Formcode = formcode[0].FirstChild.Value;
        GlobalVar.Id = id[0].FirstChild.Value;
    }

    private static string CallWebService(string xml)
    {
        XmlDocument soapEnvelopeXml = CreateSoapEnvelopeWithXML(xml);
        HttpWebRequest webRequest = CreateWebRequest(GlobalVar.Url);
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
            using (StreamReader rd = new StreamReader(webResponse.GetResponseStream(), System.Text.Encoding.UTF8, true))
            {
                soapResult = rd.ReadToEnd();
            }
            Console.Write(soapResult);
        }

        return soapResult;

    }

    private static HttpWebRequest CreateWebRequest(string url)
    {
        HttpWebRequest webRequest = (HttpWebRequest)WebRequest.Create(url);
        webRequest.Headers.Add("SOAPAction", "http://linkcare.es/LINKCARE");
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
                    " + xml + @"
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
        string error = SaveErrorLog(soapResult, "session_discover");
        string token = ParseToken(soapResult);
        return token;
    }

    public static void CallSessionGetRoleList(string token)
    {
        string xml = @"<lin:session_get_role_list soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                       </lin:session_get_role_list>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "session_get_role_list");
        found_role_id(soapResult);
    }

    public static void CallSessionRole(string token)
    {
        string xml = @"<lin:session_role soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                        <token xsi:type =""xsd:string"">" + token + @"</token>
                        <role xsi:type=""xsd:string"">" + GlobalVar.Role + @"</role>
                      </lin:session_role>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "session_role");

    }

    public static void CallSessionGetTeamList(string token)
    {
        string xml = @"<lin:session_get_team_list soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                       </lin:session_get_team_list>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "session_get_role_list");
        found_team_id(soapResult);
    }

    public static void CallSessionSetTeam(string token)
    {
        string xml = @"<lin:session_set_team soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <team xsi:type=""xsd:string"">" + GlobalVar.Team + @"</team>
                      </lin:session_set_team>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "session_set_team");
    }

    public static Dictionary<string, string>[] CallTaskListOverdue(string token)
    {
        string xml = @"<lin:task_list_overdue soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <max_res xsi:type=""xsd:string"">10</max_res>
                         <offset xsi:type=""xsd:string"">0</offset>
                      </lin:task_list_overdue>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "task_list_overdue");
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
        string error = SaveErrorLog(soapResult, "task_close");
    }

    public static string CallCaseInsert(string cip, string token)
    {
        string data = @"<?xml version=""1.0"" encoding=""UTF-8""?><case><ref></ref><given_name></given_name><family_name></family_name><refs><ref>NICK/</ref><ref>NIF/</ref><ref>NIE/</ref><ref>PAS/</ref><ref>" + GlobalVar.Id + "/" + cip + @"</ref></refs ><data><bdate></bdate><age></age><gender></gender><status></status><preferences><password_expire></password_expire></preferences></data></case>";

        string cdata = HttpUtility.HtmlEncode(data);

        string xml = @"<lin:case_insert soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                            <session xsi:type=""xsd:string"">" + token + @"</session>
                            <case xsi:type=""xsd:string"">" + cdata + @"</case>
                       </lin:case_insert>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "case_insert");
        string case_id = ParseCaseId(soapResult);
        return case_id;
    }

    public static Dictionary<string, string> CallCaseGet(string token, string case_id)
    {
        Dictionary<string, string> patient_data = new Dictionary<string, string>();

        string xml = @"<lin:case_get soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                            <session xsi:type=""xsd:string"">" + token + @"</session>
                            <case xsi:type=""xsd:string"">" + case_id + @"</case>
                            <admission xsi:type=""xsd:string""></admission>
                       </lin:case_get>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "case_get");
        patient_data = ParseDataPatient(soapResult);
        return patient_data;
    }

    public static string CallCaseForGetCip(string token, string case_id)
    {
        string xml = @"<lin:case_get soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                            <session xsi:type=""xsd:string"">" + token + @"</session>
                            <case xsi:type=""xsd:string"">" + case_id + @"</case>
                            <admission xsi:type=""xsd:string""></admission>
                       </lin:case_get>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "case_get");
        string cip = ParseCip(soapResult);
        return cip;
    }

    public static string CallAdmissionListCase(string case_id, string token)
    {
        string xml = @"<lin:admission_list_case soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <case xsi:type=""xsd:string"">" + case_id + @"</case>
                      </lin:admission_list_case>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "admission_list_case");
        string admission_id = ParseAdmissionFromCase(soapResult);
        return admission_id;
    }

    public static string CallTaskInsertByTaskCode(string admission_id, string token, string taskcode = "")
    {
        if (taskcode.Equals("")) taskcode = GlobalVar.Taskcode;

        string date_now = DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss tt");
        date_now = date_now.ToString().Replace("/", "-");

        string xml = @"<lin:task_insert_by_task_code soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <admission xsi:type=""xsd:string"">" + admission_id + @"</admission>
                         <task_code xsi:type=""xsd:string"">" + taskcode + @"</task_code>
                         <date xsi:type=""xsd:string"">" + date_now + @"</date>
                      </lin:task_insert_by_task_code>";
        string soapResult = CallWebService(xml);
        string task_id = ParseTaskId(soapResult);
        string error = SaveErrorLog(task_id, "task_insert_by_task_code");
        return task_id;
    }

    public static string CallTaskFormList(string task_id, string token, string for_request = "false")
    {
        string xml = @"<lin:task_form_list soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <task_id xsi:type=""xsd:string"">" + task_id + @"</task_id>
                       </lin:task_form_list>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "task_form_list");
        string form_id = ParseFormId(soapResult, for_request);
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
        string error = SaveErrorLog(soapResult, "form_get_summary");
        string answer_id = ParseAnswerId(soapResult);
        return answer_id;
    }

    public static void CallFormSetAnswer(string token, string form_id, string answer_id, string admission_id, string xml_to_save, string for_request = "false")
    {
        string value = "";
        if (for_request.Equals("false"))
        {
            var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(xml_to_save);
            value = admission_id + @"_ndd.xml|" + System.Convert.ToBase64String(plainTextBytes);
        }
        else
        {
            value = xml_to_save;
        }
        string xml = @"<lin:form_set_answer soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <form_id xsi:type=""xsd:string"">" + form_id + @"</form_id>
                         <question_id xsi:type=""xsd:string"">" + answer_id + @"</question_id>
                         <value xsi:type=""xsd:string"">" + value + @"</value>
                         <option_id xsi:type=""xsd:string""></option_id>
                         <event_id xsi:type=""xsd:string""></event_id>
                         <group_close xsi:type=""xsd:string""></group_close>
                         <formula_calculate xsi:type=""xsd:string""></formula_calculate>
                         <get_next_question xsi:type=""xsd:string""></get_next_question> 0
                      </lin:form_set_answer>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "form_set_answer");
    }

    public static string CallAdmissionGet(string token, string admission_id)
    {
        string xml = @"<lin:admission_get soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                         <session xsi:type=""xsd:string"">" + token + @"</session>
                         <admission xsi:type=""xsd:string"">" + admission_id + @"</admission>
                       </lin:admission_get>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "form_set_answer");
        string case_id = ParseCaseIdFormAdmission(soapResult);
        return case_id;
    }

    public static string CallFormulaExec (string token, string form_id)
    {
        string xml = @"<lin:formula_exec soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                          <session xsi:type=""xsd:string"">" + token + @"</session>
                          <form xsi:type=""xsd:string"">" + form_id + @"</form>
                          <formula xsi:type=""xsd:string"">[""GET_VALUE(CASE, DATA_CODE, WEIGHT:last)"",""GET_VALUE(CASE, DATA_CODE, HEIGHT:last)"",""GET_VALUE(CASE, DATA_CODE, ETNICITY(DESC):last)""]</formula>
                       </lin:formula_exec>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "formula_exec");
        string h_w_e = ParseFormulaExecData(soapResult);
        return h_w_e;
    }

    public static void CallSessionLanguage(string token, string language)
    {
        string xml = @"<lin:session_language soapenv:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">
                            <token xsi:type=""xsd:string"">" + token + @"</token>
                            <language xsi:type=""xsd:string"">" + language + @"</language>
                       </lin:session_language>";
        string soapResult = CallWebService(xml);
        string error = SaveErrorLog(soapResult, "session_language");
    }

    private static string ParseFormulaExecData(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("value");
        String[] data = JsonConvert.DeserializeObject<String[]>(result[0].InnerXml.ToString());
        return String.Join("|", data);
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

    private static void found_role_id(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList roles = doc.SelectNodes("roles");
        XmlNodeList role = roles[0].SelectNodes("role");
        for (int i = 0; i < role.Count; i++)
        {
            XmlNodeList role_code = role[i].SelectNodes("code");
            if (role_code[0].FirstChild.Value.Equals(GlobalVar.Role))
            {
                XmlNodeList id = role[i].SelectNodes("id");
                GlobalVar.Role = id[0].FirstChild.Value;
            }
        }
    }

    private static void found_team_id(string xmlString)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList teams = doc.SelectNodes("teams");
        XmlNodeList team = teams[0].SelectNodes("team");
        for (int i = 0; i < team.Count; i++)
        {
            XmlNodeList team_name = team[i].SelectNodes("name");
            if (team_name[0].FirstChild.Value.Equals(GlobalVar.Team))
            {
                XmlNodeList ref_team = team[i].SelectNodes("ref");
                GlobalVar.Team = ref_team[0].FirstChild.Value;
            }
        }
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
            XmlNodeList locked = admission[i].SelectNodes("locked");
            XmlNodeList data = admission[i].SelectNodes("data");
            XmlNodeList status = data[0].SelectNodes("status");
            XmlNodeList subscription = data[0].SelectNodes("subscription");
            XmlNodeList program = subscription[0].SelectNodes("program");
            XmlNodeList prog_code = program[0].SelectNodes("prog_code");
            if (prog_code[0].FirstChild.Value.Equals(GlobalVar.Progcode) && (status[0].FirstChild.Value.Equals("ACTIVE") || status[0].FirstChild.Value.Equals("ENROLLED")))
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

    private static string ParseFormId(string xmlString, string for_request)
    {
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList forms = doc.SelectNodes("forms");
        XmlNodeList form = forms[0].SelectNodes("form");
        string spiro2_form_id = "";
        if (for_request.Equals("true"))
        {
            XmlNodeList form_id = form[0].SelectNodes("ref");
            spiro2_form_id = form_id[0].FirstChild.Value;
        }
        else
        {
            for (int i = 0; i < form.Count; i++)
            {
                XmlNodeList form_code = form[i].SelectNodes("form_code");
                if (form_code[0].FirstChild.Value.Equals(GlobalVar.Formcode))
                {
                    XmlNodeList form_id = form[i].SelectNodes("ref");
                    spiro2_form_id = form_id[0].FirstChild.Value;
                }
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
    private static string SaveErrorLog(string xmlString, string ws)
    {
        if (ws.Equals("formula_exec"))
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xmlString);
            XmlNodeList ErrorMsg = doc.GetElementsByTagName("error");
            xmlString = HttpUtility.HtmlDecode(ErrorMsg[0].InnerXml.ToString());
        }
        else if (!ws.Equals("task_insert_by_task_code"))
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xmlString);
            XmlNodeList ErrorMsg = doc.GetElementsByTagName("ErrorMsg");
            xmlString = HttpUtility.HtmlDecode(ErrorMsg[0].InnerXml.ToString());
        }
        else
        {
            if (xmlString.Equals(""))
            {
                xmlString = "task not inserted";
            }
            else
            {
                xmlString = "";
            }
        }
        DateTime now = DateTime.Now;
        string date_now = now.ToString();
        string texto = "";
        if (xmlString.Equals(""))
        {
            texto = date_now + " " + ws + ": " + "completed";
        }
        else
        {
            texto = date_now + " " + ws + ": " + xmlString;
            MessageBox.Show("A problem has been detected. Please, click accept button and check the file available at " + GlobalVar.Logs);
        }
        System.IO.StreamWriter sw = new System.IO.StreamWriter(GlobalVar.Logs, true);
        sw.WriteLine(texto);
        sw.Close();
        return xmlString;
    }

    private static string ParseCaseIdFormAdmission(string xmlString)
    {
        string case_id = "";
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList admission = doc.SelectNodes("admission");
        XmlNodeList data = admission[0].SelectNodes("data");
        XmlNodeList casetag = data[0].SelectNodes("case");
        XmlNodeList caseref = casetag[0].SelectNodes("ref");
        case_id = caseref[0].FirstChild.Value;
        return case_id;
    }

    private static string ParseCip(string xmlString)
    {
        string cip = "";
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlString);
        XmlNodeList result = doc.GetElementsByTagName("result");
        xmlString = HttpUtility.HtmlDecode(result[0].InnerXml.ToString());
        doc.LoadXml(xmlString);
        XmlNodeList casetag = doc.SelectNodes("case");
        XmlNodeList refs = casetag[0].SelectNodes("refs");
        XmlNodeList reftag = refs[0].SelectNodes("ref");
        int i = 0;
        bool found = false;
        while ((i < reftag.Count) || (!found))
        {
            cip = reftag[i].FirstChild.Value;
            if (cip.Contains("CIP/"))
            {
                cip = cip.Replace("CIP/", "");
                found = true;
            }
            i++;
        }
        return cip;
    }

    private static void MakeConfigFile()
    {
        if (!File.Exists(GlobalVar.Config))
        {
            string texto = @"<?xml version=""1.0"" encoding=""UTF-8""?>
<data>
    <user>linkcarendd</user>
    <password>linkcare</password>
    <url>http://test.linkcare.es/ws/ServerWSDL.php</url>
    <role>SERVICE</role>
    <team>Linkcare</team>
    <progcode>SPIROIOT2</progcode>
    <taskcode>SPIRO:BAS</taskcode>
    <taskcodespirometryrequest>SPIRO_REQUEST</taskcodespirometryrequest>
    <formcode>SPIROS2</formcode>
    <id>CIP</id>
</data>";
            System.IO.StreamWriter sw = new System.IO.StreamWriter(GlobalVar.Config);
            sw.WriteLine(texto);
            sw.Close();
        }
    }


    public static void CheckPatient(string cip, Dictionary<string, string> data)
    {
        if (File.Exists(GlobalVar.PatientsFile))
        {
            using (StreamReader read = new StreamReader(GlobalVar.PatientsFile))
            {
                string contents = read.ReadToEnd();
                if (!contents.Contains(data["task_id"] + "|" + data["admission_id"] + "|" + data["name"] + "|" + data["surname"] + "|" + data["gender"] + "|" + data["bdate"] + "|" + cip))
                {
                    read.Close();
                    using (StreamWriter write = new StreamWriter(GlobalVar.PatientsFile, true))
                    {
                        write.WriteLine(data["task_id"] + "|" + data["admission_id"] + "|" + data["name"] + "|" + data["surname"] + "|" + data["gender"] + "|" + data["bdate"] + "|" + cip);
                    }
                }
            }
        }
        else
        {
            using (StreamWriter write = new StreamWriter(GlobalVar.PatientsFile, true))
            {
                write.WriteLine(data["task_id"] + "|" + data["admission_id"] + "|" + data["name"] + "|" + data["surname"] + "|" + data["gender"] + "|" + data["bdate"] + "|" + cip);
            }
        }
    }

    public static Dictionary<string, string>[] ReadPatientFile(int tam)
    {
        System.IO.StreamReader sr = new System.IO.StreamReader(GlobalVar.PatientsFile);
        String oneLine = sr.ReadLine();
        int i = 0;

        //recorro el fichero para ver el número de filas (pacientes) que contiene
        while (oneLine != null)
        {
            oneLine = sr.ReadLine();
            i++;
        }

        sr.DiscardBufferedData();
        sr.BaseStream.Seek(0, SeekOrigin.Begin);

        //con el número de pacientes del fichero, declaramos el tamaño de la matriz de dictionary
        Dictionary<string, string>[] data_patient = new Dictionary<string, string>[i];

        oneLine = sr.ReadLine();
        i = 0;
        while (oneLine != null)
        {
            //MessageBox.Show(oneLine);
            string[] fields = oneLine.Split('|');
            data_patient[i] = new Dictionary<string, string>();
            data_patient[i].Add("task_id", fields[0]);
            data_patient[i].Add("admission_id", fields[1]);
            data_patient[i].Add("name", fields[2]);
            data_patient[i].Add("surname", fields[3]);
            data_patient[i].Add("gender", fields[4]);
            data_patient[i].Add("bdate", fields[5]);
            data_patient[i].Add("cip", fields[6]);
            oneLine = sr.ReadLine();
            i++;
        }

        sr.Close();
        return data_patient;
    }

    public static Dictionary<string, string>[] ComparePatientsLists(Dictionary<string, string>[] patients_in_Linkcare, Dictionary<string, string>[] patients_in_File)
    {
        string[] cips_File = new string[patients_in_File.Length];
        for (int runs = 0; runs < patients_in_File.Length; runs++)
        {
            cips_File[runs] = patients_in_File[runs]["cip"];
        }

        string[] cips_Linkcare = new string[patients_in_Linkcare.Length];
        for (int runs = 0; runs < patients_in_Linkcare.Length; runs++)
        {
            cips_Linkcare[runs] = patients_in_Linkcare[runs]["cip"];
        }

        var nonIntersect = cips_File.Except(cips_Linkcare);
        Dictionary<string, string>[] data_patient = new Dictionary<string, string>[nonIntersect.LongCount()];
        int i = 0;

        foreach (string value in nonIntersect)
        {
            foreach (Dictionary<string, string> patient in patients_in_File)
            {
                if (patient["cip"] == value)
                {
                    data_patient[i] = patient;
                    i++;
                    break;
                }
            }
            //MessageBox.Show(value);
        }

        return data_patient;
        #endregion
    }

    public static void RemoveFromFile(Dictionary<string, string> data)
    {
        if (File.Exists(GlobalVar.PatientsFile))
        {
            /*
            string line = null;
            string line_to_delete = data["task_id"] + "|" + data["admission_id"] + "|" + data["name"] + "|" + data["surname"] + "|" + data["gender"] + "|" + data["bdate"] + "|" + data["cip"];

            var oStream = new FileStream(GlobalVar.PatientsFile, FileMode.Append, FileAccess.Write, FileShare.Read);
            var iStream = new FileStream(GlobalVar.PatientsFile, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);

            StreamWriter writer = new StreamWriter(oStream);
            StreamReader reader = new StreamReader(iStream);

            using (reader)
            {
                //string contents = reader.ReadToEnd();
                using (writer)
                {
                    String oneLine = reader.ReadLine();
                    while (oneLine != null)
                    {
                        if (String.Compare(oneLine, line_to_delete) == 0)
                        {
                            writer.WriteLine(line);
                            break;
                        }
                        oneLine = reader.ReadLine();
                    }
                }
            }
            */
            string tempFile = Path.GetTempFileName();
            string line_to_delete = data["task_id"] + "|" + data["admission_id"] + "|" + data["name"] + "|" + data["surname"] + "|" + data["gender"] + "|" + data["bdate"] + "|" + data["cip"];

            using (var sr = new StreamReader(GlobalVar.PatientsFile))
            using (var sw = new StreamWriter(tempFile))
            {
                string line;

                while ((line = sr.ReadLine()) != null)
                {
                    if (line != line_to_delete)
                        sw.WriteLine(line);
                }
            }

            File.Delete(GlobalVar.PatientsFile);
            File.Move(tempFile, GlobalVar.PatientsFile);
        }
    }
}
