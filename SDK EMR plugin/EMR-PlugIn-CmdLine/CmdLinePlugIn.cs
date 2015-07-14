using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Windows.Forms;
using ndd.EMR.SharedConst;

namespace EMR.PlugIn.CmdLine
{

  public class CmdLinePlugIn : InddEmrPlugIn
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
      get { return new CmdLineCfg(); }
    }

    #endregion


    #region -- Configuration Values --
    protected string m_FileExchangeFolder = System.IO.Path.GetTempPath();
    private string m_XmlExchangeFile = "ExchangeFile_" + Name + ".xml";

    public string XmlExchangeFile
    {
      get { return System.IO.Path.Combine(m_FileExchangeFolder, m_XmlExchangeFile); }
    }

    private bool _CloseAfterTest = true;

    public bool CloseAfterTest
    {
      get { return _CloseAfterTest; }
      set { _CloseAfterTest = value; }
    }

    private bool _IncludeCurveData = false;

    public bool IncludeCurveData
    {
      get { return _IncludeCurveData; }
      set { _IncludeCurveData = value; }
    }

    private bool _IncludeTrialValues = false;

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
    public CmdLinePlugIn()
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

      //example how you could use even use the command line
      UsingCommandLineParameters();

      return true;
    }

    /// <summary>
    /// Data can also be provided via command line. This reads that data and calls the appropriate command.</summary>
    private void UsingCommandLineParameters()
    {
      if (_SendXmlCallback == null)
        return;

      string[] args = Environment.GetCommandLineArgs();

      string strCommand = "";
      string strPatientID = "";
      string strDOB = "";
      string strHeight = "";
      string strTestDate= "";
      string strWeight = "";
      string strEthnicity = "";
      string strGender = "";

      for (int i = 0; i < args.Length; i++)
      {
        switch (args[i])
        {

          case "/ShowTest":
            strCommand = Commands.ShowTest.Command;
            break;

          case "/PerformTest":
            strCommand = Commands.PerformTest.Command;
            break;

          case "/PatientID":
            if (i < args.Length - 1)
            {
              strPatientID = args[i + 1];
            }
            break;
          case "/DOB":
            if (i < args.Length - 1)
            {
              strDOB = args[i + 1];
            }
            break;
          case "/Height":
            if (i < args.Length - 1)
            {
              strHeight = args[i + 1];
            }
            break;
          case "/TestDate":
            if (i < args.Length - 1)
            {
              strTestDate = args[i + 1];
            }
            break;
          case "/Gender":
            if (i < args.Length - 1)
            {
              strGender = args[i + 1];
            }
            break;
          case "/Ethnicity":
            if (i < args.Length - 1)
            {
              strEthnicity = args[i + 1];
            }
            break;
          case "/Weight":
            if (i < args.Length - 1)
            {
              strWeight = args[i + 1];
            }
            break;


          //Reference to xml command file
          case "/EMR_Cmd_File":
            if (_SendXmlCallback != null && (i < args.Length - 1))
            {
              //send command file to EWP
              _SendXmlCallback(args[i + 1]);
              return;
            }
            break;

          //send XML command as command line
          case "/EMR_Cmd_String":
            //example:EasyWarePro.exe /EMR_Cmd_String "<ndd><Command Type=\"PerformTest\"></Command><Patients><Patient ID=\"PSM 12345\"><LastName>Smith</LastName><FirstName>John</FirstName></Patient></Patients></ndd>" 
            if (_SendXmlCallback != null && (i < args.Length - 1))
            {
              //send command file to EWP
              _SendXmlCallback(args[i + 1]);
              return;
            }
            break;

          default:
            break;
        }
      }
      if (!String.IsNullOrEmpty(strCommand))
        _SendXmlCallback(String.Format(@"
            <ndd>
              <Command Type=""{0}"">
                <Parameter Name=""PatientID"">{1}</Parameter>
                <Parameter Name=""TestDate"">{4}</Parameter>
              </Command>
              <Patients>
                <Patient ID=""{0}"">
                  <LastName>Smith</LastName>
                  <FirstName>Peter</FirstName>
                  <PatientDataAtPresent>
                    <Height>{3}</Height>
                    <Weight>{5}</Weight>
                    <Ethnicity>{6}</Ethnicity>
                    <Gender>{7}</Gender>
                    <DateOfBirth>{2}</DateOfBirth>
                    <ComputedDateOfBirth>false</ComputedDateOfBirth>
                  </PatientDataAtPresent>
                </Patient>
              </Patients>
            </ndd>", strCommand, strPatientID, strDOB, strHeight, strTestDate, strWeight, strEthnicity, strGender));

    }



    protected virtual void LoadingConfigValues(Dictionary<string, string> configValues)
    {
      if (configValues.ContainsKey(UserConfig_Defs.ParamName_ExchangeFile))
      {
        m_XmlExchangeFile = configValues[UserConfig_Defs.ParamName_ExchangeFile];
      }

      if (configValues.ContainsKey(Commands.Configuration.FileExchangeFolder))
        m_FileExchangeFolder = configValues[Commands.Configuration.FileExchangeFolder];

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

    }


    /// <summary>
    /// Stops the EMR module. You could call dispose() in case you have resources to free.</summary>
    /// <remarks>required</remarks>
    public virtual void Close()
    {
    }
    #endregion

    protected struct SReceiveMsgResponse
    {
      public SReceiveMsgResponse(string strMessage, string strFileName)
      {
        m_Message = strMessage;
        m_ResponseFileName = strFileName;
      }

      private string m_Message;

      public string Message
      {
        get { return m_Message; }
        set { m_Message = value; }
      }

      private string m_ResponseFileName;

      public string ResponseFileName
      {
        get { return m_ResponseFileName; }
        set { m_ResponseFileName = value; }
      }

    }
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
          SReceiveMsgResponse sResponse = ReceiveXmlMessage(reader);

          if (String.IsNullOrEmpty(sResponse.ResponseFileName))
            return sResponse.Message;

          //copy XML file
          if (File.Exists(strPath) && strPath != sResponse.ResponseFileName)
            File.Copy(strPath, sResponse.ResponseFileName);

          return sResponse.Message;
        }
        finally
        {
          reader.Close();

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

    /// <summary>
    /// Read XML string send by EasyWarePro.</summary>
    /// <param name="strXmlMessage">message in xml format</param>
    /// <returns><c>true</c>: successfull</returns>
    /// <remarks>required</remarks>
    public string ReceiveXmlText(string strXmlMessage)
    {
      // Create the reader.
      System.Xml.XmlReader reader = System.Xml.XmlReader.Create(new System.IO.StringReader(strXmlMessage));
      SReceiveMsgResponse sResponse = ReceiveXmlMessage(reader);

      if (String.IsNullOrEmpty(sResponse.ResponseFileName))
        return sResponse.Message;

      //write XML message to file

      using (StreamWriter sw = new StreamWriter(sResponse.ResponseFileName, false, Encoding.Unicode))
      {
        if (String.IsNullOrEmpty(sResponse.Message))
          sw.Write(strXmlMessage);
        else
          sw.Write(sResponse.Message);
      }

      return "";
    }


    public virtual string ReceiveXmlMessage(string strMessage)
    {
      if (strMessage.IndexOf('<') >= 0)
        return ReceiveXmlText(strMessage);
      else
        return ReceiveXmlFile(strMessage);

      
    }

    protected SReceiveMsgResponse ReceiveXmlMessage(System.Xml.XmlReader reader)
    {
      string strXmlMessage = "";
      string strPatientID = "default";
      if (reader.ReadToFollowing("Command"))
      //if (reader.Name == "Command")
      {
        strXmlMessage = reader.GetAttribute("Type");

        //Dictionary<string, string> parameterList = new Dictionary<string, string>();
        //if (reader.ReadToDescendant("Parameter"))
        //{
        //  do
        //  {
        //    string strParameterName = reader.GetAttribute("Name");
        //    string strParameterValue = reader.ReadElementString();

        //    parameterList.Add(strParameterName, strParameterValue);
        //  } while (reader.ReadToNextSibling("Parameter"));
        //}

        switch (strXmlMessage)
        {
          case Commands.GetConfiguration.Command:
            return new SReceiveMsgResponse(ReturnConfiguration(), "");
          //break;

          case Commands.GetSupportedFeatures.Command: // "GetSupportedFeatures":
            //e.g. Worklist, CurveData, Attachment_Type, Attachment_Path
            return new SReceiveMsgResponse(ReturnSupportedFeatures(), ""); 
          ////break;

          //case Commands.SearchPatients.Command: // "SearchPatient":
          //  //list of patients
          //  return ReturnSearchPatientResult(parameterList);

          case Commands.TestResult.Command: // "TestResult":
            //TransmitData
            return ReadInfoToReturnTestResults(reader, ref strPatientID);            
            //break;

          default:
            return new SReceiveMsgResponse("", ""); 
            //break;
        }
      }

      return new SReceiveMsgResponse("", ""); 


      

    }

    private SReceiveMsgResponse ReadInfoToReturnTestResults(System.Xml.XmlReader reader, ref string strPatientID)
    {
      while (reader.Read())
      {
        if (reader.Name == "Patients")
        {
          System.Xml.XmlReader patientReader = reader.ReadSubtree();
          while (patientReader.ReadToFollowing("Patient"))
          {
            if (patientReader.AttributeCount > 0)
            {
              strPatientID = patientReader.GetAttribute("ID");
              break;
            }

            //while (patientReader.Read())
            //{
            //  if (patientReader.Name == "FirstName")
            //  {
            //    string strName = patientReader.ReadString();
            //  }
            //}

          }

          break;
        }


      }

      string strFileName = strPatientID;
      foreach (char replaceChar in Path.GetInvalidFileNameChars())
      {
        strFileName = strFileName.Replace(replaceChar, '_');
      }
      strFileName = Path.Combine(m_FileExchangeFolder, strFileName + ".xml");

      return new SReceiveMsgResponse("", strFileName);
    }

    #endregion

    #region - returning data -

    protected OnEmrSendMessageDelegate _SendXmlCallback;

    public OnEmrSendMessageDelegate SendXmlCallback
    {
      set { _SendXmlCallback = value; }
    }

    protected virtual string ReturnConfiguration()
    {

      StringBuilder sb = new System.Text.StringBuilder();

      using (System.Xml.XmlTextWriter xmlWriter = new System.Xml.XmlTextWriter(new System.IO.StringWriter(sb, CultureInfo.InvariantCulture)))
      {
        xmlWriter.WriteStartDocument();
        xmlWriter.WriteStartElement("ndd");
        xmlWriter.WriteStartElement("Command");
        xmlWriter.WriteAttributeString("Type", Commands.Configuration.Command);

        //write configuration values (they could be fix)
        WriteConfiguration(xmlWriter);

        xmlWriter.WriteEndElement();
        xmlWriter.WriteEndElement();
        xmlWriter.WriteEndDocument();

        xmlWriter.Flush();
        xmlWriter.Close();
        return sb.ToString();
      }

    }

    protected virtual void WriteConfiguration(System.Xml.XmlTextWriter xmlWriter)
    {
      WriteParameter(xmlWriter, Commands.Configuration.CloseAfterTest, _CloseAfterTest.ToString(CultureInfo.InvariantCulture));
      WriteParameter(xmlWriter, Commands.Configuration.IncludeCurveData, _IncludeCurveData.ToString(CultureInfo.InvariantCulture));
      WriteParameter(xmlWriter, Commands.Configuration.IncludeTrialValues, _IncludeTrialValues.ToString(CultureInfo.InvariantCulture));
      WriteParameter(xmlWriter, Commands.Configuration.FileExchangeFolder, m_FileExchangeFolder.ToString(CultureInfo.InvariantCulture));
    }

    protected static void WriteParameter(System.Xml.XmlTextWriter xmlWriter, string strFeature, string strValue)
    {
      xmlWriter.WriteStartElement("Parameter");
      xmlWriter.WriteAttributeString("Name", strFeature);
      xmlWriter.WriteValue(strValue);
      xmlWriter.WriteEndElement();//parameter
    }


    protected virtual List<String> GetSupportedFeatures()
    {
      //add your supported features here
      return new List<string>();
    }

    //e.g. Worklist, CurveData, Attachment_Type, Attachment_Path
    protected virtual string ReturnSupportedFeatures()
    {
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
        return sb.ToString();
      }

    }


    //e.g. Worklist, CurveData, Attachment_Type, Attachment_Path
    protected virtual string ReturnSearchPatientResult(Dictionary<string, string> parameters)
    {
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

          xmlWriter.WriteStartElement("Patient");
          xmlWriter.WriteAttributeString("ID", "SearchPatient-ID");
          xmlWriter.WriteElementString("LastName", "Search Last");
          xmlWriter.WriteElementString("FirstName", "Search First");

          xmlWriter.WriteStartElement("PatientDataAtPresent");

          xmlWriter.WriteElementString("DateOfBirth", "1956-07-28");
          xmlWriter.WriteElementString("Gender", "Female");
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
    #endregion


  }

}
