using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.Globalization;
using System.IO;
using ndd.EMR.SharedConst;

namespace EMR.PlugInTester
{
  public partial class TestForm : Form
  {
    public TestForm()
    {
      InitializeComponent();
      _SyncObject = this;
    }


    struct SEmrInfo
    {

      public SEmrInfo(string strFilename, string strName, Version version)
      {
        _FileName = strFilename;
        _Name = strName;
        _Version = version;
      }

      private string _FileName;

      public string FileName
      {
        get { return _FileName; }
        //set { m_FileName = value; }
      }

      private string _Name;

      public string Name
      {
        get { return _Name; }
        //set { m_Name = value; }
      }

      private Version _Version;

      public Version Version
      {
        get { return _Version; }
        //set { _Version = value; }
      }

  

      public override string ToString()
      {
        return String.Format("{0} [{1}]", _Name, _Version);
      }
    }

    private void LogMessage(string strMessage)
    {
      richTextBoxReceivedMessages.Text = DateTime.Now.ToShortTimeString() + " " + strMessage + Environment.NewLine + richTextBoxReceivedMessages.Text;
    }

    private void buttonGetEmr_Click(object sender, EventArgs e)
    {
      List<SEmrInfo> availableEmrList = GetAvailableEmrPlugIns();

      comboBoxAvailableEmr.Items.Clear();
      foreach (SEmrInfo emrInfo in availableEmrList)
      {
        comboBoxAvailableEmr.Items.Add(emrInfo);
      }
      if (comboBoxAvailableEmr.Items.Count > 0)
        comboBoxAvailableEmr.SelectedIndex = 0;
    }

    private static List<SEmrInfo> GetAvailableEmrPlugIns()
    {
      List<SEmrInfo> availableEmrList = new List<SEmrInfo>();

      try
      {
        string strPlugInFolder = GetEmrPlugInFolder();

        string[] aFiles = System.IO.Directory.GetFiles(strPlugInFolder, "*.dll", System.IO.SearchOption.AllDirectories);
        Dictionary<string, Guid> listedEmr = new Dictionary<string, Guid>();

        foreach (string strFile in aFiles)
        {
          try
          {




            Assembly incentiveAssembly = Assembly.LoadFrom(strFile);

            foreach (Type pluginType in incentiveAssembly.GetTypes())
            {
              //Type pluginType = incentiveAssembly.GetType("ndd.EMR.EmrPlugIn", false, true);
              //Type[] emrPluginTypes = incentiveAssembly.GetTypes();
              if (pluginType != null && pluginType.IsPublic && !pluginType.IsSealed)
              {
                bool bFoundEmrInterface = false;
                foreach (Type pluginTypInterface in pluginType.GetInterfaces())
                {
                  if (pluginTypInterface.Name == ndd.EMR.SharedConst.PlugInNaming.EmrInterfaceName)
                  {
                    bFoundEmrInterface = true;
                    break;
                  }
                  //Console.WriteLine("Interfaces of {0} -- {1}: {2}", incentiveAssembly, pluginType, pluginTypInterface);
                }

                if (!bFoundEmrInterface)
                  continue;

                //reading property NAME
                FieldInfo nameField = pluginType.GetField("Name");
                PropertyInfo nameProp = pluginType.GetProperty("Name");

                if (nameProp != null)
                {
                  //read the shown emr plug in name
                  string strPlugInName = (string)nameProp.GetValue(nameProp, BindingFlags.GetProperty, null, null, CultureInfo.CurrentCulture);

                  //check that identical module is not listed twice (e.g. copied folder, base class
                  if (!listedEmr.ContainsKey(nameProp.Module.Name) || listedEmr[nameProp.Module.Name] != nameProp.Module.ModuleVersionId)
                  {
                    availableEmrList.Add(new SEmrInfo(strFile, strPlugInName, nameProp.Module.Assembly.GetName().Version));
                    listedEmr.Add(nameProp.Module.Name, nameProp.Module.ModuleVersionId);
                  }
                }
              }
            }
          }
         
          catch (Exception ex)
          {
            MessageBox.Show(" Exception: " + ex.Message + Environment.NewLine + strFile);

          }
        }
      }
      catch (ReflectionTypeLoadException ex)
      {
        MessageBox.Show("ReflectionLoad Exception: " + ex.Message);
      }
      catch (Exception ex)
      {
        MessageBox.Show(ex.Message);
      }
      return availableEmrList;
    }


    private static string GetEmrPlugInFolder()
    {
      string strPlugInFolder = System.IO.Path.Combine(Application.StartupPath, @"..\PlugIns");
      return strPlugInFolder;
    }

    protected string OnEmrSendFile(string strPath)
    {
      richTextBoxReceivedMessages.Text = DateTime.Now.ToShortTimeString() + " received File:" + strPath + Environment.NewLine + richTextBoxReceivedMessages.Text;

      if (File.Exists(strPath))
        File.Delete(strPath);
      return null;
    }

    ISynchronizeInvoke _SyncObject;
    //public event OnEmrSendMessageDelegate OnEmrSendMessageEvent;
    public delegate string OnEmrSendMessageDelegate(string strMessage);

    public string OnEmrSendMessage(string strMessage)
    {
      if (_SyncObject.InvokeRequired)
      {
        object result = _SyncObject.Invoke(new OnEmrSendMessageDelegate(OnEmrSendMessage), new object[1] { strMessage });
        return result as String;
      }

      if (strMessage.IndexOf('<') >= 0)
        return OnEmrSendText(strMessage);
      else
        return OnEmrSendFile(strMessage);
    }

    protected string OnEmrSendText(string strMessage)
    {
      richTextBoxReceivedMessages.Text = DateTime.Now.ToShortTimeString() + " " + strMessage + Environment.NewLine + richTextBoxReceivedMessages.Text;
      return "caller return";
    }

    /*
    private void buttonSend_Click(object sender, EventArgs e)
    {
      if (comboBoxAvailableEmr.SelectedItem != null)
      {
        SEmrInfo selectedEmr = (SEmrInfo)comboBoxAvailableEmr.SelectedItem;
        Assembly startValueAssembly = Assembly.LoadFile(System.IO.Path.Combine(GetEmrPlugInFolder(), selectedEmr.FileName));

        Type plugInType = GetEmrType(startValueAssembly);

        PropertyInfo callbackSendXmlPropInfo = plugInType.GetProperty("SendXmlCallback");
        Type delegateType = callbackSendXmlPropInfo.PropertyType;

        // If you already have a method with the correct signature,
        // you can simply get a MethodInfo for it. 
          OnEmrSendMessageDelegate onEmrSendMessageEvent = this.OnEmrSendMessage;

        //Code von http://msdn.microsoft.com/en-us/library/ms228976.aspx

        // Create an instance of the delegate. Using the overloads
        // of CreateDelegate that take MethodInfo is recommended.
        //
        Delegate dSendMsg = Delegate.CreateDelegate(delegateType, this, onEmrSendMessageEvent.Method);

        callbackSendXmlPropInfo.SetValue(plugInType, dSendMsg, null);

          /*
        //MemberInfo[] memInfo = plugInType.GetMember("OnEmrSendMessage");
        //memInfo.SetValue(
        // Get an EventInfo representing the Click event, and get the
        // type of delegate that handles the event.
        //
        EventInfo evinfo = plugInType.GetEvent("OnEmrSendMessage");
        if (evinfo == null)
          return;

        Type tDelegate = evinfo.EventHandlerType;

    // If you already have a method with the correct signature,
    // you can simply get a MethodInfo for it. 
    //
    MethodInfo miHandler = 
        typeof(Form1).GetMethod("OnEmrSendMessage");
      
    // Create an instance of the delegate. Using the overloads
    // of CreateDelegate that take MethodInfo is recommended.
    //
    Delegate d = Delegate.CreateDelegate(tDelegate, this, miHandler);

    // Get the "add" accessor of the event and invoke it late-
    // bound, passing in the delegate instance. This is equivalent
    // to using the += operator in C#, or AddHandler in Visual
    // Basic. The instance on which the "add" accessor is invoked
    // is the form; the arguments must be passed as an array.
    //
    MethodInfo addHandler = evinfo.GetAddMethod();
    Object[] addHandlerArgs = { d };
    addHandler.Invoke("addHandler invoked", addHandlerArgs);



            //if (evinfo != null)
            //{
            //  MulticastDelegate eventDelegate = (MulticastDelegate)plugInType.GetType().GetField("OnEmrSendMessage", BindingFlags.Instance | BindingFlags.NonPublic).GetValue(plugInType);
            //  //if (eventDelegate != null)
            //  //{
            //  //  foreach (var handler in eventDelegate.GetInvocationList())
            //  //  {
            //  //    handler.Method.Invoke(handler.Target, new object[] { source, eventArgs });
            //  //  }
            //  //}
            //}
            //OnEmrSendMessageEvent += new OnEmrSendMessageDelegate(Form1_OnEmrSendMessageEvent);
          if(evinfo != null)
            evinfo.AddEventHandler(plugInType, d);

         Console.WriteLine("start send data");
         // evinfo.
         //result = (string)foundType.InvokeMember(function, BindingFlags.InvokeMethod, null, null, args, CultureInfo.InvariantCulture);

       }

              

        //PlugIn via Reflection:
            
       private string InvokePluginFunction(string typeName, string function, string inputXml)
               {
                   string result = null;
                   string fullDllPath = startPath + dllName;
                   try
                   {
                       Assembly startValueAssembly = Assembly.LoadFile(fullDllPath);
                       if (startValueAssembly != null)
                       {
                           Type foundType = startValueAssembly. GetType(typeName);
                           if (foundType != null)
                           {
                               object[] args = new object[] { inputXml };
                               //foundType.GetInterface(
                               result = (string)foundType.InvokeMember(function, BindingFlags.InvokeMethod, null, null, args, CultureInfo.InvariantCulture);
                           }
                           else
                           {
                               string errorMessage = string.Format("{0}: Type {1} not found!", dllName, typeName);
                               string details = string.Format("dll path = {0}\ntypeName = {1}\nfunction = {2}", fullDllPath, typeName, function);
                               MessageBox.Show(null, "Error with Plugin", errorMessage, details, MessageBoxImage.Error, false, "Close");
                           }
                       }
                   }
                   catch (Exception ex)
                   {
                       string errorMessage = string.Format("{0}: {1}", dllName, ex.Message);
                       string details = string.Format("dll path = {0}\ntypeName = {1}\nfunction = {2}", fullDllPath, typeName, function);
                       MessageBox.Show(null, "Error with Plugin ", errorMessage, details, MessageBoxImage.Error, false, "Close");
                   }
                   return result;
               }

                   
    }
*/

    private static Type GetEmrType(Assembly startValueAssembly)
    {
      try
      {
        Type resultType = null;
        foreach (Type pluginType in startValueAssembly.GetTypes())
        {
          //Type pluginType = incentiveAssembly.GetType("ndd.EMR.EmrPlugIn", false, true);
          //Type[] emrPluginTypes = incentiveAssembly.GetTypes();
          if (pluginType != null && pluginType.IsPublic && !pluginType.IsSealed)
          {
            bool bFoundEmrInterface = false;
            foreach (Type pluginTypInterface in pluginType.GetInterfaces())
            {
              if (pluginTypInterface.Name.IndexOf(ndd.EMR.SharedConst.PlugInNaming.EmrInterfaceName, StringComparison.OrdinalIgnoreCase) >= 0)
              {
                resultType = pluginType;
                bFoundEmrInterface = true;
                break;
              }
              //Console.WriteLine("Interfaces of {0} -- {1}: {2}", incentiveAssembly, pluginType, pluginTypInterface);
            }

            if (!bFoundEmrInterface)
              continue;

          }

        }
        return resultType;
      }
      //catch (System.Reflection.ReflectionTypeLoadException loadEx)
      //{
      //  //loadEx.LoaderExceptions
      //  return null;
      //}
      catch //(Exception ex)
      {
        return null;// typeof(String);
      }
    }

    protected object _LoadedEmr;
    protected MethodInfo _CloseMethod;
    protected MethodInfo _ReceiveMsgMethod;

    private void buttonInit_Click(object sender, EventArgs e)
    {
      if (comboBoxAvailableEmr.SelectedItem != null)
      {
        if (_LoadedEmr != null)
        {
          if (_CloseMethod != null)
            _CloseMethod.Invoke(_LoadedEmr, BindingFlags.InvokeMethod, null, null, null);
          
          _LoadedEmr = null;
          _CloseMethod = null;
          _ReceiveMsgMethod = null;
        }

        SEmrInfo selectedEmr = (SEmrInfo)comboBoxAvailableEmr.SelectedItem;
        string strAssemblyFileName = System.IO.Path.Combine(GetEmrPlugInFolder(), selectedEmr.FileName);
        Assembly startValueAssembly = Assembly.LoadFrom(strAssemblyFileName); //LoadFile does not work for inherited projects (can't find base dll when not in same folder as EasyWarePro)

        Type plugInType = GetEmrType(startValueAssembly);
        if (plugInType == null)
          return;

        ConstructorInfo emrConstructor = plugInType.GetConstructor(new Type[0]);
        if (emrConstructor == null)
          return;

        object emrModul = emrConstructor.Invoke(new object[0]);
        if (emrModul == null)
          return;


        _LoadedEmr = emrModul;

        #region - SendXmlCallback -
        PropertyInfo callbackSendXmlPropInfo = plugInType.GetProperty(ndd.EMR.SharedConst.PlugInNaming.EmrInterface_SendXmlCallback);
        Type delegateType = callbackSendXmlPropInfo.PropertyType;

        // If you already have a method with the correct signature,
        // you can simply get a MethodInfo for it. 

        //following code is replaced with: onEmrSendMessageEvent.Method
        //MethodInfo miHandlerSendMsg =
        //    typeof(Form1).GetMethod("OnEmrSendMessage");
        OnEmrSendMessageDelegate onEmrSendMessageEvent = this.OnEmrSendMessage;

        //Code von http://msdn.microsoft.com/en-us/library/ms228976.aspx

        // Create an instance of the delegate. Using the overloads
        // of CreateDelegate that take MethodInfo is recommended.
        //
        Delegate dSendMsg = Delegate.CreateDelegate(delegateType, this, onEmrSendMessageEvent.Method);

        callbackSendXmlPropInfo.SetValue(emrModul, dSendMsg, null);
        #endregion

        //load method
        _ReceiveMsgMethod = plugInType.GetMethod(ndd.EMR.SharedConst.PlugInNaming.EmrInterface_ReceiveXmlMessage);
       
        //close method
        _CloseMethod = plugInType.GetMethod(ndd.EMR.SharedConst.PlugInNaming.EmrInterface_Close);

        #region - call method Initialize -

        MethodInfo methodInit = plugInType.GetMethod("Initialize");
        if (methodInit == null)
          return;

        if (_ConfigValues == null)
          _ConfigValues = new Dictionary<string, string>();
        object[] attr = new object[] { _ConfigValues };
        bool result = (bool)methodInit.Invoke(emrModul, attr);

        if (!result)
          return;
        #endregion
      }
    }

    private void buttonGetConfiguration_Click(object sender, EventArgs e)
    {
      if (_ReceiveMsgMethod == null)
        return;

      //prepare arguments
      string strXml = @"
<ndd>
    <Command Type=""GetConfiguration"">
    </Command>
</ndd>";

      CallReceiveMsgMethod(strXml);



    }


    private void buttonGetFeatures_Click(object sender, EventArgs e)
    {
      if (_ReceiveMsgMethod == null)
        return;

      //prepare arguments
      string strXml = @"
<ndd>
    <Command Type=""GetSupportedFeatures"">
    </Command>
</ndd>";
      object[] args = new object[] { strXml };
      //Type[] methodTypes = new Type[] { typeof(string) };

      //Aufruf
      //Variante 1
      object result = _ReceiveMsgMethod.Invoke(_LoadedEmr, BindingFlags.InvokeMethod, null, args, null);

      if (result != null)
      {
        LogMessage(result as String);
      }
      else
      {
        LogMessage("<>");
      }

    }

    private void buttonSendData_Click(object sender, EventArgs e)
    {
      if (_ReceiveMsgMethod == null)
        return;

      //prepare arguments
      string strXml = @"
<ndd>
    <Command Type=""TestResult"">
        <Parameter Name=""ParameterA"">Test passing values</Parameter>
        <Parameter Name=""ParameterB"">Test passing values</Parameter>
    </Command>
<Patients>
                 <Patient ID=""TestPatientID33"">
      <LastName>My lastname</LastName>
      <FirstName>My firstname</FirstName>
      <IsBioCal>false</IsBioCal>
      <PatientDataAtPresent>
        <Height>1.7</Height>
        <Weight>70</Weight>
        <Ethnicity>Caucasian</Ethnicity>
        <Smoker />
        <Asthma />
        <Gender>Female</Gender>
        <DateOfBirth>1972-01-27</DateOfBirth>
        <ComputedDateOfBirth>false</ComputedDateOfBirth>
        <COPD />
      </PatientDataAtPresent>
      <Intervals>
        <Interval>
          <OrderID />
          <LungAge>39</LungAge>
          <Tests>
            <Test TypeOfTest=""FVL"">
              <TestDate>2011-01-27T16:28:16.985</TestDate>
              <BestValues>
                <ResultParameter ID=""FVC"">
                  <DataValue>3.7481629848480225</DataValue>
                  <Unit>L</Unit>
                  <PredictedValue>3.6268749189330265</PredictedValue>
                  <LLNormalValue>2.9193749347468838</LLNormalValue>
                </ResultParameter>
                <ResultParameter ID=""PEF"">
                  <DataValue>11.015999794006348</DataValue>
                  <Unit>L/s</Unit>
                  <PredictedValue>7.0699998419731855</PredictedValue>
                  <LLNormalValue>5.5893748750677332</LLNormalValue>
                </ResultParameter>
                <ResultParameter ID=""FEV1"">
                  <DataValue>3.5915015935897827</DataValue>
                  <Unit>L</Unit>
                  <PredictedValue>3.1399999298155308</PredictedValue>
                  <LLNormalValue>2.5143749437993392</LLNormalValue>
                </ResultParameter>
                <ResultParameter ID=""FEV1_VCmax"">
                  <DataValue>0.9582031539472684</DataValue>
                  <Unit />
                  <PredictedValue>NaN</PredictedValue>
                  <LLNormalValue>NaN</LLNormalValue>
                </ResultParameter>
                <ResultParameter ID=""FEV1_FVC"">
                  <DataValue>0.9582031539472684</DataValue>
                  <Unit />
                  <PredictedValue>0.81600003875792027</PredictedValue>
                  <LLNormalValue>0.70800003362819552</LLNormalValue>
                </ResultParameter>
              </BestValues>
              <SWVersion>01.03.00.9999</SWVersion>
              <PatientDataAtTestTime>
                <Height>1.7</Height>
                <Weight>70</Weight>
                <Ethnicity>Caucasian</Ethnicity>
                <Smoker />
                <Asthma />
                <Gender>Female</Gender>
                <DateOfBirth>1972-01-27</DateOfBirth>
                <ComputedDateOfBirth>false</ComputedDateOfBirth>
                <COPD />
              </PatientDataAtTestTime>
              <QualityGradeOriginal>D1</QualityGradeOriginal>
              <QualityGrade>D1</QualityGrade>
              <Trials>
                <Trial>
                  <Date>2011-01-27T16:28:36.572</Date>
                  <Number>1</Number>
                  <Rank>3</Rank>
                  <RankOriginal>3</RankOriginal>
                  <Accepted>true</Accepted>
                  <AcceptedOriginal>true</AcceptedOriginal>
                  <ManualAmbientOverride>false</ManualAmbientOverride>
                </Trial>
                <Trial>
                  <Date>2011-01-27T16:29:02.209</Date>
                  <Number>2</Number>
                  <Rank>2</Rank>
                  <RankOriginal>2</RankOriginal>
                  <Accepted>true</Accepted>
                  <AcceptedOriginal>true</AcceptedOriginal>
                  <ManualAmbientOverride>false</ManualAmbientOverride>
                </Trial>
                <Trial>
                  <Date>2011-01-27T16:29:18.644</Date>
                  <Number>3</Number>
                  <Rank>1</Rank>
                  <RankOriginal>1</RankOriginal>
                  <Accepted>true</Accepted>
                  <AcceptedOriginal>true</AcceptedOriginal>
                  <ManualAmbientOverride>false</ManualAmbientOverride>
                </Trial>
              </Trials>
              <Device>
                <SerialNumber>190073</SerialNumber>
                <DeviceType>SPIROSON_AS</DeviceType>
              </Device>
            </Test>
          </Tests>
        </Interval>
      </Intervals>
    </Patient>
  </Patients>
</ndd>";

      CallReceiveMsgMethod(strXml);


      //Variante 2
      //object result2 = plugInType.InvokeMember("ReceiveMessage", BindingFlags.InvokeMethod, null, null, args, CultureInfo.InvariantCulture);
    }

    private void buttonSendFile_Click(object sender, EventArgs e)
    {
      if (_ReceiveMsgMethod == null)
        return;

      //temporär
      string strFilePath = System.IO.Path.Combine(Path.GetTempPath(), "FileFromTesterToPlugIn" + ".xml"); //Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "ExchangeFile.xml");
      using (System.Xml.XmlTextWriter xmlWriter = new System.Xml.XmlTextWriter(strFilePath, UTF8Encoding.UTF8))
      {
        string strCommand = "TestResults";
        xmlWriter.WriteStartDocument();
        xmlWriter.WriteStartElement("ndd");
        xmlWriter.WriteRaw(@"
                <Command Type=""" + strCommand + @""">
                  <Parameter Name=""Attachment""></Parameter>
                </Command>");
        xmlWriter.WriteRaw(@"<Patients>
                 <Patient ID=""TestPatientID33"">
      <LastName>My lastname</LastName>
      <FirstName>My firstname</FirstName>
      <IsBioCal>false</IsBioCal>
      <PatientDataAtPresent>
        <Height>1.7</Height>
        <Weight>70</Weight>
        <Ethnicity>Caucasian</Ethnicity>
        <Smoker />
        <Asthma />
        <Gender>Female</Gender>
        <DateOfBirth>1972-01-27</DateOfBirth>
        <ComputedDateOfBirth>false</ComputedDateOfBirth>
        <COPD />
      </PatientDataAtPresent>
      <Intervals>
        <Interval>
          <OrderID />
          <LungAge>39</LungAge>
          <Tests>
            <Test TypeOfTest=""FVL"">
              <TestDate>2011-01-27T16:28:16.985</TestDate>
              <BestValues>
                <ResultParameter ID=""FVC"">
                  <DataValue>3.7481629848480225</DataValue>
                  <Unit>L</Unit>
                  <PredictedValue>3.6268749189330265</PredictedValue>
                  <LLNormalValue>2.9193749347468838</LLNormalValue>
                </ResultParameter>
                <ResultParameter ID=""PEF"">
                  <DataValue>11.015999794006348</DataValue>
                  <Unit>L/s</Unit>
                  <PredictedValue>7.0699998419731855</PredictedValue>
                  <LLNormalValue>5.5893748750677332</LLNormalValue>
                </ResultParameter>
                <ResultParameter ID=""FEV1"">
                  <DataValue>3.5915015935897827</DataValue>
                  <Unit>L</Unit>
                  <PredictedValue>3.1399999298155308</PredictedValue>
                  <LLNormalValue>2.5143749437993392</LLNormalValue>
                </ResultParameter>
                <ResultParameter ID=""FEV1_VCmax"">
                  <DataValue>0.9582031539472684</DataValue>
                  <Unit />
                  <PredictedValue>NaN</PredictedValue>
                  <LLNormalValue>NaN</LLNormalValue>
                </ResultParameter>
                <ResultParameter ID=""FEV1_FVC"">
                  <DataValue>0.9582031539472684</DataValue>
                  <Unit />
                  <PredictedValue>0.81600003875792027</PredictedValue>
                  <LLNormalValue>0.70800003362819552</LLNormalValue>
                </ResultParameter>
              </BestValues>
              <SWVersion>01.03.00.9999</SWVersion>
              <PatientDataAtTestTime>
                <Height>1.7</Height>
                <Weight>70</Weight>
                <Ethnicity>Caucasian</Ethnicity>
                <Smoker />
                <Asthma />
                <Gender>Female</Gender>
                <DateOfBirth>1972-01-27</DateOfBirth>
                <ComputedDateOfBirth>false</ComputedDateOfBirth>
                <COPD />
              </PatientDataAtTestTime>
              <QualityGradeOriginal>D1</QualityGradeOriginal>
              <QualityGrade>D1</QualityGrade>
              <Trials>
                <Trial>
                  <Date>2011-01-27T16:28:36.572</Date>
                  <Number>1</Number>
                  <Rank>3</Rank>
                  <RankOriginal>3</RankOriginal>
                  <Accepted>true</Accepted>
                  <AcceptedOriginal>true</AcceptedOriginal>
                  <ManualAmbientOverride>false</ManualAmbientOverride>
                </Trial>
                <Trial>
                  <Date>2011-01-27T16:29:02.209</Date>
                  <Number>2</Number>
                  <Rank>2</Rank>
                  <RankOriginal>2</RankOriginal>
                  <Accepted>true</Accepted>
                  <AcceptedOriginal>true</AcceptedOriginal>
                  <ManualAmbientOverride>false</ManualAmbientOverride>
                </Trial>
                <Trial>
                  <Date>2011-01-27T16:29:18.644</Date>
                  <Number>3</Number>
                  <Rank>1</Rank>
                  <RankOriginal>1</RankOriginal>
                  <Accepted>true</Accepted>
                  <AcceptedOriginal>true</AcceptedOriginal>
                  <ManualAmbientOverride>false</ManualAmbientOverride>
                </Trial>
              </Trials>
              <Device>
                <SerialNumber>190073</SerialNumber>
                <DeviceType>SPIROSON_AS</DeviceType>
              </Device>
            </Test>
          </Tests>
        </Interval>
      </Intervals>
    </Patient>
  </Patients>");

        //xmlWriter.WriteStartElement("Patient");
        //xmlWriter.WriteAttributeString("ID", "PSM-xxx");
        //xmlWriter.WriteElementString("LastName", "Smith");
        //xmlWriter.WriteElementString("FirstName", "Peter");
        //xmlWriter.WriteEndElement();
        
        xmlWriter.WriteEndElement(); //</ndd>
        xmlWriter.WriteEndDocument();

        xmlWriter.Flush();
        xmlWriter.Close();
      }

      CallReceiveMsgMethod(strFilePath);

    }

    Dictionary<string, string> _ConfigValues;

    private void buttonShowCfg_Click(object sender, EventArgs e)
    {
      if (comboBoxAvailableEmr.SelectedItem != null)
      {
        SEmrInfo selectedEmr = (SEmrInfo)comboBoxAvailableEmr.SelectedItem;
        Assembly startValueAssembly = Assembly.LoadFrom(System.IO.Path.Combine(GetEmrPlugInFolder(), selectedEmr.FileName));

        Type plugInType = GetEmrType(startValueAssembly);

        if (plugInType == null)
          return;

        PropertyInfo propConfigControl = plugInType.GetProperty(ndd.EMR.SharedConst.PlugInNaming.EmrInterface_CfgCtrl);

        if (propConfigControl != null)
        {
          Control controlCfg = propConfigControl.GetValue(propConfigControl, BindingFlags.GetProperty, null, null, CultureInfo.CurrentCulture) as Control;
          if (controlCfg != null)
          {
            //remove previous controls
            while (panelCfgHolder.Controls.Count > 0)
            {
              panelCfgHolder.Controls.RemoveAt(0);
            }

            panelCfgHolder.Controls.Add(controlCfg);

            //load values
            MethodInfo methodGetGuiValues = controlCfg.GetType().GetMethod(ndd.EMR.SharedConst.PlugInNaming.EmrCfgCtrlInterface_SetGuiValues);
            if (methodGetGuiValues == null)
              return;

            if (_ConfigValues == null)
              _ConfigValues = new Dictionary<string, string>();

            methodGetGuiValues.Invoke(controlCfg, new object[1] { _ConfigValues });

          }
        }

      }
    }

    private void buttonSaveCfg_Click(object sender, EventArgs e)
    {
      if (panelCfgHolder.Controls.Count <= 0)
        return;

      Type typeCfgControl = panelCfgHolder.Controls[0].GetType();
      MethodInfo methodValidate = typeCfgControl.GetMethod(ndd.EMR.SharedConst.PlugInNaming.EmrCfgCtrlInterface_ValidateEntries);
      if (methodValidate == null)
        return;
      bool bIsValid = (bool)methodValidate.Invoke(panelCfgHolder.Controls[0], new object[0]);

      if (bIsValid)
      {
        MethodInfo methodGetGuiValues = typeCfgControl.GetMethod(ndd.EMR.SharedConst.PlugInNaming.EmrCfgCtrlInterface_GetGuiValues);
        if (methodGetGuiValues == null)
          return;
        _ConfigValues = methodGetGuiValues.Invoke(panelCfgHolder.Controls[0], new object[0]) as Dictionary<string, string>;
        if (_ConfigValues == null)
          return;


        Directory.CreateDirectory(Path.GetDirectoryName(CfgFile));
        using (StreamWriter dictWriter = new StreamWriter(CfgFile))
        {
          foreach (KeyValuePair<string, string> value in _ConfigValues)
          {
            //write to file
            dictWriter.WriteLine(string.Format("{0}={1}", value.Key, value.Value));

            //write to GUI
            richTextBoxReceivedMessages.AppendText(value.Key + ": " + value.Value);
            richTextBoxReceivedMessages.AppendText(Environment.NewLine);
          }

          dictWriter.Flush();
        }
        
        //"close" GUI
        //remove controls
        while (panelCfgHolder.Controls.Count > 0)
        {
          panelCfgHolder.Controls.RemoveAt(0);
        }

      }

    }

    public string CfgFile
    {
      get
      {
        SEmrInfo selectedEmr = (SEmrInfo)comboBoxAvailableEmr.SelectedItem;
        return Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData), @"ndd\" + Application.ProductName + @"\" + selectedEmr.Name + ".cfg");
      }
    }

    private void buttonLoadCfg_Click(object sender, EventArgs e)
    {
      if (!File.Exists(CfgFile))
      {
        MessageBox.Show("No Config-File found!"+Environment.NewLine+ CfgFile);
        return;
      }

      //load config from file
      _ConfigValues = new Dictionary<string, string>();

      using (StreamReader dictWriter = new StreamReader(CfgFile))
      {
        while (!dictWriter.EndOfStream)
        {
          string strLine = dictWriter.ReadLine();

          string[] strKeyValues= strLine.Split('=');

          if (strKeyValues.Length == 2)
          {
            _ConfigValues.Add(strKeyValues[0], strKeyValues[1]);
          }
        }
      }
    }

    private void buttonSearchEMR_Click(object sender, EventArgs e)
    {
      if (_ReceiveMsgMethod == null)
        return;

      //prepare arguments
      string strXml = String.Format(@"<ndd><Command Type=""{0}"">", ndd.EMR.SharedConst.Commands.SearchPatients.Command);

      if(!String.IsNullOrEmpty(textBoxPatientID.Text))
       strXml += String.Format(@"<Parameter Name=""{0}"">{1}</Parameter>", ndd.EMR.SharedConst.Commands.SearchPatients.PatientID,textBoxPatientID.Text) ;

            if(!String.IsNullOrEmpty(textBoxLastname.Text))
       strXml += String.Format(@"<Parameter Name=""{0}"">{1}</Parameter>", ndd.EMR.SharedConst.Commands.SearchPatients.LastName,textBoxLastname.Text) ;

            if(!String.IsNullOrEmpty(textBoxFirstName.Text))
       strXml += String.Format(@"<Parameter Name=""{0}"">{1}</Parameter>", ndd.EMR.SharedConst.Commands.SearchPatients.FirstName,textBoxFirstName.Text) ;


        strXml +=String.Format(@"</Command><Patients>
    <Patient ID=""{0}"">
      <LastName>{1}</LastName>
      <FirstName>{2}</FirstName>
    </Patient>
</Patients>
</ndd>",textBoxPatientID.Text, textBoxLastname.Text, textBoxFirstName.Text);
     
      CallReceiveMsgMethod(strXml);


    }

    private void CallReceiveMsgMethod(string strXml)
    {
      object[] args = new object[] { strXml };
      //Type[] methodTypes = new Type[] { typeof(string) };

      //Aufruf
      //Variante 1
      object result = _ReceiveMsgMethod.Invoke(_LoadedEmr, BindingFlags.InvokeMethod, null, args, null);

      string strMessage = result as String;
      richTextBoxReceivedMessages.Text = DateTime.Now.ToShortTimeString() + " returnMsg: " + strMessage + Environment.NewLine + richTextBoxReceivedMessages.Text;
    }

    private void buttonSyncPatient_Click(object sender, EventArgs e)
    {
      if (_ReceiveMsgMethod == null)
        return;

      StringBuilder sb = new System.Text.StringBuilder();

      using (System.Xml.XmlTextWriter xmlWriter = new System.Xml.XmlTextWriter(new System.IO.StringWriter(sb, CultureInfo.InvariantCulture)))
      {
        xmlWriter.WriteStartDocument();
        xmlWriter.WriteStartElement("ndd");
        AddCommand(Commands.TestResult.Command, xmlWriter, Commands.ShowTest.OrderID, textBoxOrderID.Text);

        xmlWriter.WriteStartElement("Patients");
        AddGuiPatient(xmlWriter);
        xmlWriter.WriteEndElement();//Patients
        xmlWriter.WriteEndElement();
        xmlWriter.WriteEndDocument();

        xmlWriter.Flush();
        xmlWriter.Close();
      }

      CallReceiveMsgMethod(sb.ToString());

    }

    private static void AddCommand(string strCommand, System.Xml.XmlTextWriter xmlWriter)
    {
      xmlWriter.WriteStartElement("Command");
      xmlWriter.WriteAttributeString("Type", strCommand);
      xmlWriter.WriteEndElement();//command
    }

    private static void AddCommand(string strCommand, System.Xml.XmlTextWriter xmlWriter, string strParamName, string strParamValue)
    {
      xmlWriter.WriteStartElement("Command");
      xmlWriter.WriteAttributeString("Type", strCommand);

      xmlWriter.WriteStartElement("Parameter");
      xmlWriter.WriteAttributeString("Name", strParamName);
      xmlWriter.WriteValue(strParamValue);
      xmlWriter.WriteEndElement();//Param

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
    }



  }
}