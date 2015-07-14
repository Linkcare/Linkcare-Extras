using System.Collections.Generic;
using System.Globalization;
using System.Text;
using System.Windows.Forms;
using EMR.PlugIn.Basic;
using ndd.EMR.SharedConst;
using System;

namespace EMR.PlugIn.TestBench
{
  /// <summary>
  /// This plug in is for testing purpose only. </summary>
  public class TestBenchPlugIn : EMR.PlugIn.Basic.EmrPlugIn
  {

    #region - PlugIn Information - static -

    /// <summary>
    /// Name shown in the EMR selection drop down list.</summary>
    /// <remarks>required</remarks>
    public static new string Name
    {
      get { return Properties.Resources.PlugInName; }
    }
    #endregion

    #region - PlugIn Configuration - static -
    /// <summary>
    /// Control containing GUI elements to configure plugIn settings.</summary>
    /// <remarks>required when PlugIn is configurable</remarks>
    public static new Control ConfigurationControl
    {
      //not supported in this example
      get { return new UserControlEmrConfig(); }
      //get { return null; }
    }


    #endregion

    #region - Constructor, Initialization, Close -

    /// <summary>
    /// Constructor, parameterless</summary>
    ///<remarks>required: parametereless</remarks>
    public TestBenchPlugIn()
    {
    }

    protected TestBenchForm _benchForm;
    /// <summary>
    /// Initialization and starting EMR Module</summary>
    /// <param name="configValues">saved configuration values, <see cref="UserControlEmrConfig"/></param>
    /// <returns>true if successfull</returns>
    /// <remarks>required</remarks>
    public override bool Initialize(Dictionary<string, string> configValues)
    {
      //load saved configuration values
      base.Initialize(configValues);

      #region - show the TestBencForm -
      _benchForm = new TestBenchForm(this);

      //example to show show the test bench during the initialization of the EMR Plug-In only (before EasyWarePro is fully shown)
      //_benchForm.ShowDialog(); //==> you need to add a Close button to the TestBenchForm!!
      
      //show form in parallel to EasyWarePro
      _benchForm.Show();

      ////example how to bring the form to front (after EasyWarePro started up)
      //System.Threading.Tasks.Task delayedTask = new System.Threading.Tasks.Task(() => { System.Threading.Thread.Sleep(1500); _benchForm.BringToFront(); });
      //delayedTask.Start();
      #endregion
      

      return true;
    }

    /// <summary>
    /// Stops the EMR module. You could call dispose() in case you have resources to free.</summary>
    /// <remarks>required</remarks>
    public override void Close()
    {
      if (_benchForm != null)
      {
        if (_benchForm.InvokeRequired)
        {
          _benchForm.Invoke(new Action(Close));
          return;
        }
        _benchForm.Close();
      }

      base.Close();

      _benchForm = null;
    }


    protected override void LoadingConfigValues(Dictionary<string, string> configValues)
    {
      base.LoadingConfigValues(configValues);
    }


    /// <summary>
    /// return plugin configuration values, which are relevant for EasyWarePro</summary>
    /// <param name="xmlWriter"></param>
    protected override void WriteConfigurationValues(System.Xml.XmlTextWriter xmlWriter)
    {
      base.WriteConfigurationValues(xmlWriter);

      //the parameters below are not configurable through the EasyWarePro configuration (not in UserCtrlEmrCfg)
      WriteParameter(xmlWriter, Commands.Configuration.AttachReport, Properties.Settings.Default.Config_AttachPDF.ToString(CultureInfo.InvariantCulture));
      WriteParameter(xmlWriter, Commands.Configuration.AttachmentFileName, Properties.Settings.Default.AttachmentFileNamePart1 + Properties.Settings.Default.AttachmentFileNamePart2 + Properties.Settings.Default.AttachmentFileNamePart3);
      WriteParameter(xmlWriter, Commands.Configuration.AttachmentFormat, Properties.Settings.Default.AttachmentFormat);
    }

    protected override List<string> GetSupportedFeatures()
    {
      List<string> features = base.GetSupportedFeatures();

      if (Properties.Settings.Default.Supported_SearchPatients)
        features.Add(Commands.SupportedFeatures.SearchPatients);

      if (Properties.Settings.Default.Supported_Worklist)
        features.Add(Commands.SupportedFeatures.Worklist);

      return features;

    }

    #endregion

    #region - handling receiving data -
    /// <summary>
    /// handle messages from EasyWarePro</summary>
    /// <param name="strMessage">assumption: starting with '&lt;' is a xml string; else path to XML file</param>
    /// <returns>Response to EasyWarePro</returns>
    public override string ReceiveXmlMessage(string strMessage)
    {
      //log
      if (OnLogMessage != null)
        OnLogMessage("Received data from EasyWarePro:" + System.Environment.NewLine + strMessage);

      return base.ReceiveXmlMessage(strMessage);
    }
    #endregion

    #region - returning data -
 
    /// <summary>
    /// Creates XML string which represents search patient result</summary>
    /// <param name="parameters">query parameter</param>
    protected override string ReturnSearchPatientResult(Dictionary<string, string> parameters)
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

          _benchForm.AddGuiPatient(xmlWriter);
          _benchForm.AddGuiPatient(xmlWriter);
          _benchForm.AddGuiPatient(xmlWriter);

          xmlWriter.WriteEndElement();//Patients
          xmlWriter.WriteEndElement();
          xmlWriter.WriteEndDocument();
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

    #region - Connection for the Test Bench Form -
    public delegate void OnReceivingMessageDelegate(string strMessage);
    public OnReceivingMessageDelegate OnLogMessage;

    /// <summary>
    /// Log messages send to EasyWarePro</summary>
    public override string SendMessage(string strMessage)
    {
      //log
      if (OnLogMessage != null)
        OnLogMessage("Sent data to EasyWarePro:" + System.Environment.NewLine + strMessage);

      //send data
      string result = base.SendMessage(strMessage);

      //log
      if (OnLogMessage != null)
        OnLogMessage("Received data from EasyWarePro:" + System.Environment.NewLine + result);
      return result;
    }
  }
    #endregion


}

