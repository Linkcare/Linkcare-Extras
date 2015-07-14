using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using System.Windows.Forms;
using ndd.EMR.SharedConst;

namespace EMR.PlugIn.TestBench
{
  public partial class TestBenchForm : Form
  {
    TestBenchPlugIn _emrPlugIn;

    public TestBenchForm(TestBenchPlugIn emrPlugIn)
    {
      InitializeComponent();

      _emrPlugIn = emrPlugIn;
      _emrPlugIn.OnLogMessage = ReceivingMessage;

      labelVersionInfo.Text = String.Format("Version: {0}", System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString());
    }

    delegate void ReceivingMessageDelegate(string strMessage);
    private void ReceivingMessage(string strMessage)
    {
      if (this.InvokeRequired)
      {
        Invoke(new ReceivingMessageDelegate(ReceivingMessage), new object[] { strMessage });
        return;
      }

      richTextBoxReceivedMessages.Text = DateTime.Now.ToShortTimeString() + " " + strMessage + Environment.NewLine + richTextBoxReceivedMessages.Text;


      // ndd internal code
      if (textBoxPatientID.Text.StartsWith("Exception", StringComparison.CurrentCultureIgnoreCase))
        throw new Exception("This ReceivingMessage exception is for testing purpose");
    }


    private void tabPageSettings_Enter(object sender, EventArgs e)
    {
      checkBoxIncludeCurveData.Checked = this._emrPlugIn.IncludeCurveData;
      checkBoxIncludeTrialValues.Checked = this._emrPlugIn.IncludeTrialValues;
      checkBoxCloseAfterTest.Checked = _emrPlugIn.CloseAfterTest;
      textBoxEchangeFolder.Text = _emrPlugIn.XmlExchangeFile;
    }

    private void EMRTestForm_FormClosing(object sender, FormClosingEventArgs e)
    {
      Properties.Settings.Default.Save();
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

    public void AddGuiPatient(System.Xml.XmlWriter xmlWriter)
    {
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
        if(Double.TryParse(maskedTextBoxHeight.Text, out dHeight))
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
    }

    private void SendCommand(string command, Dictionary<string, string> cmdParameters)
    {

      StringBuilder sb = new System.Text.StringBuilder();

      using (System.Xml.XmlTextWriter xmlWriter = new System.Xml.XmlTextWriter(new System.IO.StringWriter(sb, CultureInfo.InvariantCulture)))
      {
        xmlWriter.WriteStartDocument();
        xmlWriter.WriteStartElement("ndd");

        AddCommand(command, xmlWriter, cmdParameters);

        xmlWriter.WriteStartElement("Patients");
        AddGuiPatient(xmlWriter);
        xmlWriter.WriteEndElement();//Patients
        xmlWriter.WriteEndElement();
        xmlWriter.WriteEndDocument();

        xmlWriter.Flush();
        xmlWriter.Close();
      }
      _emrPlugIn.SendMessage(sb.ToString());

      // ndd internal code
      if (textBoxPatientID.Text.StartsWith("Exception", StringComparison.CurrentCultureIgnoreCase))
        throw new Exception("This Exception is for testing purpose");
    }

    private void buttonAddPatient_Click(object sender, EventArgs e)
    {
      var cmdParameters = new Dictionary<string, string>();
      SendCommand(Commands.SyncPatient.Command, cmdParameters);
    }

    private void buttonAddWorklist_Click(object sender, EventArgs e)
    {
      var cmdParameters = new Dictionary<string, string>();
      //if (!String.IsNullOrEmpty(textBoxOrderID.Text))
      cmdParameters.Add(Commands.AddToWorklist.OrderID, textBoxOrderID.Text);

      SendCommand(Commands.AddToWorklist.Command, cmdParameters);
    }

    private void buttonPerformTest_Click(object sender, EventArgs e)
    {
      var cmdParameters = new Dictionary<string, string>();
      //if (!String.IsNullOrEmpty(textBoxOrderID.Text))
        cmdParameters.Add(Commands.PerformTest.OrderID, textBoxOrderID.Text);

      SendCommand(Commands.PerformTest.Command, cmdParameters);
    }

    private void buttonPerformTestType_Click(object sender, EventArgs e)
    {
      var cmdParameters = new Dictionary<string, string>();
      //if (!String.IsNullOrEmpty(textBoxOrderID.Text))
      cmdParameters.Add(Commands.PerformTest.OrderID, textBoxOrderID.Text);
      cmdParameters.Add(Commands.PerformTest.TestType, comboBoxTestType.Text);

      SendCommand(Commands.PerformTest.Command, cmdParameters);
    }

    private void buttonShowTest_Click(object sender, EventArgs e)
    {
      var cmdParameters = new Dictionary<string, string>();
      cmdParameters.Add(Commands.ShowTest.OrderID, textBoxOrderID.Text);

      SendCommand(Commands.ShowTest.Command, cmdParameters);
    }


    private void buttonGetTest_Click(object sender, EventArgs e)
    {
      var cmdParameters = new Dictionary<string, string>();
      if (!String.IsNullOrEmpty(textBoxTestTime.Text))
        cmdParameters.Add(Commands.GetTestResults.DateTime, textBoxTestTime.Text);

      SendCommand(Commands.GetTestResults.Command, cmdParameters);
    }

    private void buttonPrintPreview_Click(object sender, EventArgs e)
    {
      var cmdParameters = new Dictionary<string, string>();
      cmdParameters.Add(Commands.PrintTest.DateTime, textBoxTestTime.Text);
      cmdParameters.Add(Commands.PrintTest.ShowPreview, "True");

      SendCommand(Commands.PrintTest.Command, cmdParameters);
    }


    private void buttonPrint_Click(object sender, EventArgs e)
    {
      var cmdParameters = new Dictionary<string, string>();
      cmdParameters.Add(Commands.PrintTest.DateTime, textBoxTestTime.Text);

      SendCommand(Commands.PrintTest.Command, cmdParameters);
    }



 

  }
}