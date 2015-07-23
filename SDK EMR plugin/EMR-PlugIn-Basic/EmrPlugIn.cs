using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Windows.Forms;
using ndd.EMR.SharedConst;

namespace EMR.PlugIn.Basic
{

  public class EmrPlugIn : InddEmrPlugIn
  {
  
    #region - PlugIn Information - static -

    /// <summary>
    /// Name shown in the EMR selection drop down list.</summary>
     /// <remarks>required</remarks>
    public static string Name
    {
      get { return Properties.Resources.PlugInName; }
    }
    #endregion

    #region - PlugIn Configuration - static -
    /// <summary>
    /// Control containing GUI elements to configure plugIn settings.</summary>
    /// <remarks>required when PlugIn is configurable</remarks>
    public static Control ConfigurationControl
    {
      get { return new UserControlEmrConfig(); }
    }

    #endregion

    private FileSystemWatcher _fileWatcher;


    #region -- Configuration Values --
    protected string _FileExchangeFolder = System.IO.Path.GetTempPath();
    private string m_XmlExchangeFile = "ExchangeFile_PlugIn-EasyWarePro.xml";


    public string XmlExchangeFile
    {
      get { return System.IO.Path.Combine(_FileExchangeFolder, m_XmlExchangeFile); }
    }


    public bool _UseCmdFile = false;

    public bool UseCmdFile
    {
      get
      {
        return _UseCmdFile && !String.IsNullOrEmpty(_CmdFileIn) && !String.IsNullOrEmpty(_CmdFileOut);
      }
      set
      {
        _UseCmdFile = value;
      }
    }

    private string _CmdFileIn;

    public string CmdFileIn
    {
      get { return _CmdFileIn; }
      set { _CmdFileIn = value; }
    }

    private string _CmdFileOut;

    public string CmdFileOut
    {
      get { return _CmdFileOut; }
      set { _CmdFileOut = value; }
    }

    #region - Attachments -
    private string _CfgAttachReport_String;
    private string _CfgAttachmentFormat;
    private string _CfgAttachmentFileName;
    #endregion

    private bool _CloseAfterTest;

    public bool CloseAfterTest
    {
      get { return _CloseAfterTest; }
      set { _CloseAfterTest = value; }
    }

    private bool _SkipPatientConfirmation;

    public bool SkipPatientConfirmation
    {
      get { return _SkipPatientConfirmation; }
      set { _SkipPatientConfirmation = value; }
    }

    private bool _IncludeCurveData;

    public bool IncludeCurveData
    {
      get { return _IncludeCurveData; }
      set { _IncludeCurveData = value; }
    }

    private bool _IncludeTrialValues;

    public bool IncludeTrialValues
    {
      get { return _IncludeTrialValues; }
      set { _IncludeTrialValues = value; }
    }

    #endregion


    #region - Constructor, Initialization, Close -

    /// <summary>
    /// Constructor, parameterless</summary>
    ///<remarks>required: parametereless</remarks>
    public EmrPlugIn()
    {
    }

    /// <summary>
    /// Initialization and starting EMR Module</summary>
    /// <param name="configValues">saved configuration values, <see cref="UserControlEmrConfig"/></param>
    /// <returns>true if successfull</returns>
    /// <remarks>required</remarks>
    public virtual bool Initialize(Dictionary<string, string> configValues)
    {
      //load saved configuration values
      LoadingConfigValues(configValues);

      //configuration must be sent to EasyWarePro prior sending commands like PerformTest
      //sending them here fullfills that requirement (this a workaround of a known limitation of the current interface)
      SendMessage(ReturnConfiguration());
      SendMessage(ReturnSupportedFeatures());

      //starts watching for files, this could initiate a call
      StartPlugIn();

      //PROGRAMAR
      string token = SoapClient.CallSessionDiscover();
      Console.Write("Este es el token:" + token);
      Dictionary<string, string>[] data = SoapClient.CallTaskListOverdue(token);
      var cmdParameters = new Dictionary<string, string>();

      //if (!String.IsNullOrEmpty(textBoxOrderID.Text))

      //por cada una de las tareas, hacer un AddWorkList
      for (int i = 0; i < data.Length; i++){
            cmdParameters.Clear();
            cmdParameters.Add(Commands.AddToWorklist.OrderID, data[i]["task_id"]);
            SendCommand(Commands.AddToWorklist.Command, cmdParameters, data[i]);
      }

      //cerrar cada una de las tareas
      //for (int i = 0; i < data.Length; i++)
      //{
      //      SoapClient.CallTaskClose(data[i]["task_id"], token);
      //}

      //Dictionary<string, string> parameterList = new Dictionary<string, string>();
      //parameterList.Add("key", "value");
      //string patients_finded = ReturnSearchPatientResult(parameterList);

      return true;
    }

    /// <summary>
    /// Activating the plugIn</summary>
    protected void StartPlugIn()
    {
      if (UseCmdFile)
      {
        _fileWatcher = new FileSystemWatcher(_FileExchangeFolder);
        _fileWatcher.NotifyFilter = NotifyFilters.FileName;
        _fileWatcher.Filter = _CmdFileIn;
        _fileWatcher.IncludeSubdirectories = false;
        _fileWatcher.Created += new FileSystemEventHandler(fileWatcher_Created);
        //_fileWatcher.Changed += new FileSystemEventHandler(_fileWatcher_Changed);
        _fileWatcher.EnableRaisingEvents = true;

        string strFileIn = Path.Combine(_FileExchangeFolder, CmdFileIn);
        if (File.Exists(strFileIn))
        {
          SendMessage(strFileIn);
        }
      }
    }

    /// <summary>
    /// load configuration values</summary>
    /// <param name="configValues"></param>
    protected virtual void LoadingConfigValues(Dictionary<string, string> configValues)
    {

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_UseCmdFile))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[UserConfig_Defs.ParamName_UseCmdFile], out bValue))
          _UseCmdFile = bValue;
      }

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_CmdFileIn))
      {
        _CmdFileIn = configValues[UserConfig_Defs.ParamName_CmdFileIn];
      }

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_CmdFileOut))
      {
        _CmdFileOut = configValues[UserConfig_Defs.ParamName_CmdFileOut];
      }

      if (configValues.ContainsKey(Commands.Configuration.FileExchangeFolder))
        _FileExchangeFolder = configValues[Commands.Configuration.FileExchangeFolder];


      #region - Attachments -
      if (configValues.ContainsKey(Commands.Configuration.AttachReport))
      {
        _CfgAttachReport_String = configValues[Commands.Configuration.AttachReport];
      }

      if (configValues.ContainsKey(Commands.Configuration.AttachmentFormat))
      {
        _CfgAttachmentFormat = configValues[Commands.Configuration.AttachmentFormat];
      }

      if (configValues.ContainsKey(Commands.Configuration.AttachmentFileName))
      {
        _CfgAttachmentFileName = configValues[Commands.Configuration.AttachmentFileName];
      }
      #endregion

      if (configValues.ContainsKey(Commands.Configuration.IncludeCurveData))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.IncludeCurveData], out bValue))
          _IncludeCurveData = bValue;
      }

      if (configValues.ContainsKey(Commands.Configuration.IncludeTrialValues))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.IncludeTrialValues], out bValue))
          _IncludeTrialValues = bValue;
      }

      if (configValues.ContainsKey(Commands.Configuration.CloseAfterTest))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.CloseAfterTest], out bValue))
          _CloseAfterTest = bValue;
      }

      if (configValues.ContainsKey(Commands.Configuration.SkipPatientSelectionConfirmation))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.SkipPatientSelectionConfirmation], out bValue))
          _SkipPatientConfirmation = bValue;
      }
    }


    /// <summary>
    /// Stops the EMR module. You could call dispose() in case you have resources to free.</summary>
    /// <remarks>required</remarks>
    public virtual void Close()
    {
    }
    #endregion

    #region - handling sending data -

    /// <summary>
    /// Send XML data to EasyWarePro</summary>
    /// <param name="strMessage">xml string</param>
    /// <returns>response from EasyWarePro</returns>
    public virtual string SendMessage(string strMessage)
    {
      if (_SendXmlCallback != null)
      {
        return _SendXmlCallback(strMessage);
      }
      return "";
    }


    /// <summary>
    /// This is an example how you could watch a folder and forward a command file to EasyWarePro when it is created.</summary>
    void fileWatcher_Created(object sender, FileSystemEventArgs e)
    {
      if ((e == null) || (e.FullPath == null))
        return;

      if (e.ChangeType == WatcherChangeTypes.Created)
      {
        //make sure you send only command files which are addressed for EasyWarePro
        //e.g. the name defined here could be an optional setting
        //..  if (e.Name == _CmdFileIn)

        _fileWatcher.EnableRaisingEvents = false;
         
        //wait other application to close the file
        System.Threading.Thread.Sleep(300);
        //check file still exists
        FileInfo objFileInfo = new FileInfo(e.FullPath);
        if (!objFileInfo.Exists) 
          return;    // ignore the file open
        
        SendMessage(e.FullPath);
        
        _fileWatcher.EnableRaisingEvents = true;
      }
    }

    #endregion


    #region - handling receiving data -

    /// <summary>
    /// Read XML file send by EasyWarePro.</summary>
    /// <param name="strPath">full path to the xml file</param>
    /// <returns><c>true</c>: successfull</returns>
    /// <remarks>required</remarks>
    protected string ReceiveXmlFile(string strPath)
    {
      // Create the reader.
      using (System.Xml.XmlReader reader = new System.Xml.XmlTextReader(strPath))
      {

        try
        {
          //Example of data processing handled within this PlugIn
          string strReturnMsg = ReceiveXmlMessage(reader);
          return strReturnMsg;
        }
        finally
        {
          reader.Close();

          if (UseCmdFile)
          {
            //example of data processing handled outside of this plugIn
            try
            {
              FileInfo fi = new FileInfo(strPath);
              if (fi.Exists)
              {
                fi.MoveTo(Path.Combine(_FileExchangeFolder, _CmdFileOut));
              }
            }
            catch { }
          }
          else
          {
            //someone has to delete the file
            //delete file
            try
            {
              if (File.Exists(strPath))
                File.Delete(strPath);
            }
            catch { }
          }


        }
      }
    }

    /// <summary>
    /// Read XML string send by EasyWarePro.</summary>
    /// <param name="strXmlMessage">message in xml format</param>
    /// <returns><c>true</c>: successfull</returns>
    /// <remarks>required</remarks>
    public string ReceiveXmlText(string strXmlMessage)
    {
      // Create the reader.
      System.Xml.XmlReader reader = System.Xml.XmlReader.Create(new System.IO.StringReader(strXmlMessage));
      string strResponse = ReceiveXmlMessage(reader);

      if (UseCmdFile) //if messages are handle outside, the above ReceiveXmlMessage() needs to be adapted
      {
        //write XML message to file
        using (StreamWriter sw = new StreamWriter(Path.Combine(_FileExchangeFolder, CmdFileOut), false, Encoding.Unicode))
        {
            sw.Write(strXmlMessage);
        }
      }

      return strResponse;
    }

    /// <summary>
    /// handle messages from EasyWarePro</summary>
    /// <param name="strMessage">assumption: starting with '&lt;' is a xml string; else path to XML file</param>
    /// <returns>Response to EasyWarePro</returns>
    public virtual string ReceiveXmlMessage(string strMessage)
    {
      if (strMessage.IndexOf('<') >= 0)
        return ReceiveXmlText(strMessage);
      else
        return ReceiveXmlFile(strMessage);
    }

    /// <summary>
    /// Handle messages sent by EasyWarePro</summary>
    /// <returns>Response to EasyWarePro</returns>
    protected string ReceiveXmlMessage(System.Xml.XmlReader reader)
    {
      string strCommand = "";

      if (reader.ReadToFollowing("Command"))
      //if (reader.Name == "Command")
      {
        strCommand = reader.GetAttribute("Type");

        Dictionary<string, string> parameterList = new Dictionary<string, string>();
        if (reader.ReadToDescendant("Parameter"))
        {
          do
          {
            string strParameterName = reader.GetAttribute("Name");
            string strParameterValue = reader.ReadElementString();

            parameterList.Add(strParameterName, strParameterValue);
          } while (reader.ReadToNextSibling("Parameter"));
        }

        switch (strCommand)
        {
          case Commands.Configuration.Command:
            //load saved configuration values
            LoadingConfigValues(parameterList);
            return "";

          case Commands.GetConfiguration.Command:
            return ReturnConfiguration();
          //break;

          case Commands.GetSupportedFeatures.Command: // "GetSupportedFeatures":
            //e.g. Worklist, CurveData, Attachment_Type, Attachment_Path
            return ReturnSupportedFeatures();
          //break;
          case Commands.SearchPatients.Command: // "SearchPatient":
            //list of patients
            return ReturnSearchPatientResult(parameterList);

          case Commands.TestResult.Command: // "TestResult":
            //TransmitData
            //TODO: Add your code here!!!!!!!!!!!
            MessageBox.Show("Returning test results to your EMR is not implemented.");
            return "";
          default:
            return @"
              <ndd>
                <Command Type=""Error"">
                  <Parameter Name=""Message"">Command not supported</Parameter>
                  <Parameter Name=""Details"">" + strCommand + @"</Parameter>
                </Command>
              </ndd>";
       
        }
      }

      while (reader.Read())
      {
        if (reader.Name == "Patients")
        {
          System.Xml.XmlReader patientReader = reader.ReadSubtree();
          while (patientReader.ReadToFollowing("Patient"))
          {
            if (patientReader.ReadAttributeValue() && patientReader.AttributeCount > 0)
            {
              string strPatientID = patientReader.GetAttribute("ID");
              string strPatientNone = patientReader.GetAttribute("NotAvailable");
            }

            while (patientReader.Read())
            {
              if (patientReader.Name == "FirstName")
              {
                string strName = patientReader.ReadString();
              }
            }

          }
          if (reader.ReadToFollowing("Patient"))
          {

          }
        }


      }

      //Console.Beep();
      return "";
    }

    #endregion

    #region - returning data -

    protected OnEmrSendMessageDelegate _SendXmlCallback;

    /// <summary>
    /// EasyWarePro attaches a callback method to send data to EasyWarePro</summary>
    public OnEmrSendMessageDelegate SendXmlCallback
    {
      set { _SendXmlCallback = value; }
    }

    /// <summary>
    /// Returns the configuration as XML string</summary>
    protected virtual string ReturnConfiguration()
    {
      //- example -
      //<?xml version="1.0" encoding="UTF-16"?>
      //<ndd>
      //  <Command Type="Configuration">
      //    <Parameter Name="CloseAfterTest">True</Parameter>
      //    <Parameter Name="SkipPatientSelectionConfirmation">True</Parameter>
      //    <Parameter Name="IncludeCurveData">False</Parameter>
      //    <Parameter Name="IncludeTrialValues">False</Parameter>
      //    <Parameter Name="FileExchangeFolder">C:\temp\EMR</Parameter>
      //    <Parameter Name="AttachReport">True</Parameter>
      //    <Parameter Name="AttachmentFileName">%PatientID%</Parameter>
      //    <Parameter Name="AttachmentFormat">PDF</Parameter>
      //</Command>
      //</ndd>

      StringBuilder sb = new System.Text.StringBuilder();

      using (System.Xml.XmlTextWriter xmlWriter = new System.Xml.XmlTextWriter(new System.IO.StringWriter(sb, CultureInfo.InvariantCulture)))
      {
        xmlWriter.WriteStartDocument();
        xmlWriter.WriteStartElement("ndd");
        xmlWriter.WriteStartElement("Command");
        xmlWriter.WriteAttributeString("Type", Commands.Configuration.Command);

        //write configuration values (they could be fix)
        WriteConfigurationValues(xmlWriter);

        xmlWriter.WriteEndElement();
        xmlWriter.WriteEndElement();
        xmlWriter.WriteEndDocument();

        xmlWriter.Flush();
        xmlWriter.Close();
        return sb.ToString();
      }
    }

    /// <summary>
    /// Adds the configuration values</summary>
    protected virtual void WriteConfigurationValues(System.Xml.XmlTextWriter xmlWriter)
    {
      WriteParameter(xmlWriter, Commands.Configuration.CloseAfterTest, _CloseAfterTest.ToString(CultureInfo.InvariantCulture));
      WriteParameter(xmlWriter, Commands.Configuration.SkipPatientSelectionConfirmation, _SkipPatientConfirmation.ToString(CultureInfo.InvariantCulture));
      WriteParameter(xmlWriter, Commands.Configuration.IncludeCurveData, _IncludeCurveData.ToString(CultureInfo.InvariantCulture));
      WriteParameter(xmlWriter, Commands.Configuration.IncludeTrialValues, _IncludeTrialValues.ToString(CultureInfo.InvariantCulture));
      WriteParameter(xmlWriter, Commands.Configuration.FileExchangeFolder, _FileExchangeFolder.ToString(CultureInfo.InvariantCulture));

      WriteParameter(xmlWriter, Commands.Configuration.AttachReport, _CfgAttachReport_String);
      WriteParameter(xmlWriter, Commands.Configuration.AttachmentFileName, _CfgAttachmentFileName);
      WriteParameter(xmlWriter, Commands.Configuration.AttachmentFormat, _CfgAttachmentFormat);
    }

    /// <summary>
    /// Adds a single configuration parameter</summary>
    /// <param name="strParameterName">Parameter identifier</param>
    /// <param name="strValue">Value of the parameter</param>
    protected static void WriteParameter(System.Xml.XmlTextWriter xmlWriter, string strParameterName, string strValue)
    {
      xmlWriter.WriteStartElement("Parameter");
      xmlWriter.WriteAttributeString("Name", strParameterName);
      xmlWriter.WriteValue(strValue);
      xmlWriter.WriteEndElement();//parameter
    }

    /// <summary>
    /// Returns the list of supported features</summary>
    /// <returns></returns>
    protected virtual List<String> GetSupportedFeatures()
    {
        //add your supported features here
        List<string> features = new List<string>();
        features.Add(Commands.SupportedFeatures.Worklist);
        features.Add(Commands.SupportedFeatures.SearchPatients);
        return features;
    }

    /// <summary>
    /// Returns the list of features this plug-in supports: like Worklist, CurveData, Attachment_Type, Attachment_Path</summary>
    protected virtual string ReturnSupportedFeatures()
    {
      //- Example -
      //<?xml version="1.0" encoding="utf-16"?>
      //<ndd>
      //    <Command Type="SupportedFeatures">
      //        <Parameter Name="SearchPatients"></Parameter>
      //    </Command>
      //</ndd>


      StringBuilder sb = new System.Text.StringBuilder();

      using (System.Xml.XmlTextWriter xmlWriter = new System.Xml.XmlTextWriter(new System.IO.StringWriter(sb, CultureInfo.InvariantCulture)))
      {
        xmlWriter.WriteStartDocument();
        xmlWriter.WriteStartElement("ndd");
        xmlWriter.WriteStartElement("Command");
        xmlWriter.WriteAttributeString("Type", Commands.SupportedFeatures.Command);

        foreach (string strFeature in GetSupportedFeatures())
        {
          xmlWriter.WriteStartElement("Parameter");
          xmlWriter.WriteAttributeString("Name", strFeature);
          xmlWriter.WriteValue("True");
          xmlWriter.WriteEndElement();//parameter

        }
        xmlWriter.WriteEndElement();
        xmlWriter.WriteEndElement();
        xmlWriter.WriteEndDocument();
        xmlWriter.Flush();
        xmlWriter.Close();
        return sb.ToString();
      }

    }


    /// <summary>
    /// Example of SearchPatient Result</summary>
    /// <param name="parameters">Search criterias</param>
    protected virtual string ReturnSearchPatientResult(Dictionary<string, string> parameters)
    {
      //TODO: add your query here !!!

      string token = SoapClient.CallSessionDiscover();
      string cip = parameters["PatientID"];
      string case_id = SoapClient.CallCaseInsert(cip, token);
      Dictionary<string, string> patient_finded = SoapClient.CallCaseGet(case_id, token);

      patient_finded.Add("admission_id", cip);

      var cmdParameters = new Dictionary<string, string>();
      cmdParameters.Clear();
      cmdParameters.Add(Commands.AddToWorklist.OrderID, patient_finded["admission_id"]);
      SendCommand(Commands.AddToWorklist.Command, cmdParameters, patient_finded);

      using (System.Xml.XmlWriter xmlWriter = new System.Xml.XmlTextWriter(XmlExchangeFile, Encoding.UTF8))
      {
        try
        {
          xmlWriter.WriteStartDocument();
          xmlWriter.WriteStartElement("ndd");
          xmlWriter.WriteStartElement("Command");
          xmlWriter.WriteAttributeString("Type", Commands.SearchPatientsResult.Command);
          xmlWriter.WriteEndElement();//command
          xmlWriter.WriteStartElement("Patients");

          //TODO: fill patient list with your data !!
          MessageBox.Show("Querying EMR system for patients is not implemented.");
          xmlWriter.WriteStartElement("Patient");
          xmlWriter.WriteAttributeString("ID", patient_finded["admission_id"]);
          xmlWriter.WriteElementString("LastName", patient_finded["fullname"]);
          xmlWriter.WriteElementString("FirstName", patient_finded["nickname"]);

          xmlWriter.WriteStartElement("PatientDataAtPresent");

          xmlWriter.WriteElementString("DateOfBirth", patient_finded["bdate"]);
          xmlWriter.WriteElementString("Gender", (patient_finded["gender"].Equals("M")) ? "Male" : "Female");
          xmlWriter.WriteElementString("Height", "1.82");
          xmlWriter.WriteElementString("Weight", "64");
          xmlWriter.WriteElementString("Ethnicity", "Caucasian");
          xmlWriter.WriteEndElement();//PatientDataAtPresent

          xmlWriter.WriteEndElement();//Patient
          xmlWriter.WriteEndElement();//Patients
          xmlWriter.WriteEndElement();
          xmlWriter.WriteEndDocument();
          //                xmlWriter.WriteString(@"
          //<ndd>
          //<command>Test xml data</command><Patients>
          //    <Patient ID=""PSM-11213"">
          //      <LastName>Smith</LastName>
          //      <FirstName>Peter</FirstName></Patient>
          //</Patients>
          //</ndd>");
          xmlWriter.Flush();
          xmlWriter.Close();

          return XmlExchangeFile;
        }
        finally
        {
          xmlWriter.Close();
        }
      }
    }

    private void SendCommand(string command, Dictionary<string, string> cmdParameters, Dictionary<string, string> data)
    {

        StringBuilder sb = new System.Text.StringBuilder();

        using (System.Xml.XmlTextWriter xmlWriter = new System.Xml.XmlTextWriter(new System.IO.StringWriter(sb, CultureInfo.InvariantCulture)))
        {
            xmlWriter.WriteStartDocument();
            xmlWriter.WriteStartElement("ndd");

            AddCommand(command, xmlWriter, cmdParameters);

            xmlWriter.WriteStartElement("Patients");

            AddGuiPatient(xmlWriter, data);
            xmlWriter.WriteEndElement();//Patients
            xmlWriter.WriteEndElement();
            xmlWriter.WriteEndDocument();

            xmlWriter.Flush();
            xmlWriter.Close();

            Console.Write(xmlWriter);
        }
            SendMessage(sb.ToString());

            // ndd internal code
            //if (textBoxPatientID.Text.StartsWith("Exception", StringComparison.CurrentCultureIgnoreCase))
            //    throw new Exception("This Exception is for testing purpose");
        }

    private static void AddCommand(string strCommand, System.Xml.XmlTextWriter xmlWriter)
    {
        AddCommand(strCommand, xmlWriter, new Dictionary<string, string>());
    }

    private static void AddCommand(string strCommand, System.Xml.XmlTextWriter xmlWriter, string strParamName, string strParamValue)
    {
        var cmdParameters = new Dictionary<string, string>();
        cmdParameters.Add(strParamName, strParamValue);

        AddCommand(strCommand, xmlWriter, cmdParameters);
    }

    private static void AddCommand(string strCommand, System.Xml.XmlTextWriter xmlWriter, params KeyValuePair<string, string>[] cmdParameters)
    {
        var cmdParameterList = new Dictionary<string, string>();
        foreach (var cmdParameter in cmdParameters)
        {
            cmdParameterList.Add(cmdParameter.Key, cmdParameter.Value);
        }

        AddCommand(strCommand, xmlWriter, cmdParameterList);
    }

    private static void AddCommand(string strCommand, System.Xml.XmlTextWriter xmlWriter, Dictionary<string, string> cmdParameters)
    {
        xmlWriter.WriteStartElement("Command");
        xmlWriter.WriteAttributeString("Type", strCommand);

        foreach (var cmdParameter in cmdParameters)
        {
            xmlWriter.WriteStartElement("Parameter");
            xmlWriter.WriteAttributeString("Name", cmdParameter.Key);
            xmlWriter.WriteValue(cmdParameter.Value);
            xmlWriter.WriteEndElement();//Param        
        }

        xmlWriter.WriteEndElement();//command
    }

    public void AddGuiPatient(System.Xml.XmlWriter xmlWriter, Dictionary<string, string> data)
    {
        xmlWriter.WriteStartElement("Patient");
        xmlWriter.WriteAttributeString("ID", data["admission_id"]);
        xmlWriter.WriteElementString("LastName", data["fullname"]);
        xmlWriter.WriteElementString("FirstName", data["fullname"]); 
        xmlWriter.WriteStartElement("PatientDataAtPresent");
        xmlWriter.WriteElementString("DateOfBirth", data["bdate"]);
        xmlWriter.WriteElementString("Gender", (data["gender"].Equals("M")) ? "Male" : "Female"); 
        xmlWriter.WriteElementString("Height", "1.75");
        xmlWriter.WriteElementString("Weight", "75");
        xmlWriter.WriteElementString("Ethnicity", "Caucasian");
        xmlWriter.WriteEndElement();//PatientDataAtPresent
        xmlWriter.WriteEndElement();//Patient

            /*
        //<Patient ID=""PSM-11213"">
        //      <LastName>Smith</LastName>
        //      <FirstName>Peter</FirstName>
        //</Patient>
        xmlWriter.WriteStartElement("Patient");
        if (!String.IsNullOrEmpty(textBoxPatientID.Text))
        {
            xmlWriter.WriteAttributeString("ID", textBoxPatientID.Text);
        }
        if (!String.IsNullOrEmpty(textBoxLastname.Text))
        {
            xmlWriter.WriteElementString("LastName", textBoxLastname.Text);
        }
        if (!String.IsNullOrEmpty(textBoxLastname.Text))
        {
            xmlWriter.WriteElementString("FirstName", textBoxFirstName.Text);
        }

        xmlWriter.WriteStartElement("PatientDataAtPresent");
        if (!String.IsNullOrEmpty(dateTimePicker1.Text))
        {
            xmlWriter.WriteElementString("DateOfBirth", dateTimePicker1.Value.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture));
        }
        if (!String.IsNullOrEmpty(comboBoxGender.Text))
        {
            xmlWriter.WriteElementString("Gender", comboBoxGender.Text);
        }
        if (!String.IsNullOrEmpty(maskedTextBoxHeight.Text))
        {
            double dHeight;
            if (Double.TryParse(maskedTextBoxHeight.Text, out dHeight))
                xmlWriter.WriteElementString("Height", dHeight.ToString(CultureInfo.InvariantCulture));
        }
        if (!String.IsNullOrEmpty(numericUpDownWeight.Text))
        {
            xmlWriter.WriteElementString("Weight", numericUpDownWeight.Text);
        }
        if (!String.IsNullOrEmpty(comboBoxEthnicity.Text))
        {
            xmlWriter.WriteElementString("Ethnicity", comboBoxEthnicity.Text);
        }
        xmlWriter.WriteEndElement();//PatientDataAtPresent

        xmlWriter.WriteEndElement();//Patient
        */
    }
    #endregion
    }
}
