#region -- File Info --
//==============================================================================
// Copyright (c) by ndd Medizintechnik Zuerich, Switzerland.All rights reserved.
// -----------------------------------------------------------------------------
// Project: EasyWarePro EMR Plug in
// Content: shared definitions, constant declaration
// -----------------------------------------------------------------------------
// RCS Version Information:
// $HeadURL: svn://nddsrv02/software/Projects/PC/EasyWarePro/branches/EMR_nddOwner/SDK%20EMR%20PlugIn/EMR-PlugIn-Basic/SharedConst.cs $
// $LastChangedBy: mzinniker $
// $Date: 2014-04-24 16:00:01 +0200 (Do, 24 Apr 2014) $
// $Revision: 48076 $
//==============================================================================
#endregion


//*********************************************************************
//* 
//* This definitions are shared with 3rd party EMR providers! 
//* Avoid changes and consider additions instead.
//*
//*********************************************************************
namespace ndd.EMR.SharedConst
{
  /// <summary>
  /// Names of properties and methods which are expected in the Plug-In</summary>
  public static class PlugInNaming
  {
    public static string EmrInterfaceName                    = "InddEmrPlugIn";

    //static properties
    public static string EmrInterface_Name                   = "Name";
    public static string EmrInterface_CfgCtrl                = "ConfigurationControl";

    //methods
    public static string EmrInterface_SendXmlCallback        = "SendXmlCallback";
    public static string EmrInterface_ReceiveXmlMessage      = "ReceiveXmlMessage";
    public static string EmrInterface_Initialize             = "Initialize";
    public static string EmrInterface_Close                  = "Close";

    //methods to be supported by the config GUI
    public static string EmrCfgCtrlInterface_SetGuiValues    = "SetGuiValues";
    public static string EmrCfgCtrlInterface_ValidateEntries = "ValidateEntries";
    public static string EmrCfgCtrlInterface_GetGuiValues    = "GetGuiValues";
  }

  /// <summary>
  /// Defines the commands ndd supports.</summary>
  public static class Commands
  {
    /// <summary>
    /// EasyWarePro.exe sends this command to get the configuration from the Plug-In. Return this request with the <see cref="Configuration"/> command.</summary>
    public static class GetConfiguration
    {
      public const string Command = "GetConfiguration";
    }


    /// <summary>
    /// Definitions used to send configurations to EasyWarePro.exe</summary>
    public class Configuration
    {
      /// <summary>
      /// Identifies the configuration setting command.</summary>
      public const string Command              = "Configuration";

      /// <summary>
      /// Setting to define where files sent between EasyWarePro <-> and Plug-In should be saved at.</summary>
      public const string FileExchangeFolder   = "FileExchangeFolder";

      /// <summary>
      /// Flag whether user interaction is allowed when commands like AddToWorklist, GetTestResults, SyncPatient is called.</summary>
      /// <value>True = User does not interact, events between EWP and EMR is unattended (one cannot show a GUI to ask a question what to do)</value>
      public const string WorkflowIsBackground = "WorkflowIsBackground";
      
      /// <summary>
      /// Flag whether the patient demographic screen (Edit patient) is shown for confirmation purpose.</summary>
      /// <remarks>supporte in EWP version >= 1.5.0.18</remarks>
      public const string SkipPatientSelectionConfirmation = "SkipPatientSelectionConfirmation";

      /// <summary>
      /// Flag whether EasyWarePro should close when test results are sent to plug-in.</summary>
      public const string CloseAfterTest       = "CloseAfterTest";

      /// <summary>
      /// Flag whether test results should contain curve data points.</summary>
      /// <remarks>Currently flow and volume curves of spirometry tests are supported only.</remarks>
      public const string IncludeCurveData     = "IncludeCurveData";

      /// <summary>
      /// Flag whether test results should contain the result parameter of each trial.</summary>
      public const string IncludeTrialValues   = "IncludeTrialValues";

      #region - Report Attachment Configuration -
      /// <summary>
      /// Flag whether report (print out) should be attached to returned test results. The report is 
      /// created as a separate file and the path is added to the returned xml.</summary>
      public const string AttachReport         = "AttachReport";

      /// <summary>
      /// Setting to define the format of the attached report (PDF, RTF, PNG, TIFF, ..).</summary>
      public const string AttachmentFormat     = "AttachmentFormat";

      /// <summary>
      /// Setting to define the filename of the attached report. Available place holder are listed below.</summary>
      public const string AttachmentFileName   = "AttachmentFileName";

      /// <summary>
      /// Is replaced by the ID of the patient.</summary>
      public const string AttachmentFileName_PlaceHolder_PatientID = "%PatientID%";

      /// <summary>
      /// Is replaced by the firstname of the patient.</summary>
      public const string AttachmentFileName_PlaceHolder_Firstname = "%Firstname%";

      /// <summary>
      /// Is replaced by the lastname of the patient.</summary>
      public const string AttachmentFileName_PlaceHolder_Lastname  = "%Lastname%";

      /// <summary>
      /// Is replaced by the order ID (when available).</summary>
      /// <remarks>can be provided via <see cref="AddToWorklist.OrderID"/> and <see cref="PerformTest.OrderID"/></remarks>
      public const string AttachmentFileName_PlaceHolder_OrderID   = "%OrderID%";

      /// <summary>
      /// Is replaced by date of birth</summary>
      /// <remarks>supported since EWP version >= 1.3.0.5</remarks>
      public const string AttachmentFileName_PlaceHolder_DOB       = "%DOB%";

      /// <summary>
      /// Is replaced by Machine Name</summary>
      /// <remarks>supported in EWP version >= 1.8.0.6</remarks>
      public const string AttachmentFileName_PlaceHolder_Computer  = "%Computer%";

      /// <summary>
      /// Is replaced by test date/time in the format yyyyMMddHHmmssfff</summary>
      /// <remarks>supported in EWP version >= 1.8.0.6</remarks>
      public const string AttachmentFileName_PlaceHolder_TestTime  = "%TestTime%";

      /// <summary>
      /// Is replaced by serial number of the device that performed the test</summary>
      /// <remarks>supported since EWP version >= 1.8.0.6</remarks>
      public const string AttachmentFileName_PlaceHolder_DeviceSN = "%DeviceSN%";
      #endregion
      
    }

    /// <summary>
    /// EasyWarePro.exe sends this command to get the supported feature list (e.g. SearchPatients) from the Plug-In. Return this request with the <see cref="SupportedFeatures"/> command.</summary>
    public static class GetSupportedFeatures
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command = "GetSupportedFeatures";
    }

    /// <summary>
    /// Definitions used to send supported features to EasyWarePro.exe</summary>
    /// <example>
    /// <ndd>
    ///   <Command Type="SupportedFeatures">
    ///     <Parameter Name="SearchPatients">False</Parameter>
    ///   </Command>
    /// </ndd>
    /// </example>
    public class SupportedFeatures
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command        = "SupportedFeatures";

      /// <summary>
      /// Flag whether application should be closed after EMR session.</summary>
      public const string Worklist       = "Worklist";

      /// <summary>
      /// Flag whether the PlugIn supports the SearchPatients command. If True the filter in the patients list view in EWP will be replaced by Search EMR.</summary>
      public const string SearchPatients = "SearchPatients";
    }

    /// <summary>
    /// Adds new patient(s) to the DB (in case that patient ID does not already exists), else updates patient data (like name). </summary>
    public static class SyncPatient
    {
      public const string Command = "SyncPatient";
    }

    /// <summary>
    /// Adds the patient (in case that patient ID does not already exist) and the order to the DB.</summary>
    public static class AddToWorklist
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command   = "AddToWorklist";

      /// <summary>
      /// EMR specific identification which must be unique.</summary>
      public const string OrderID   = "OrderID";

      /// <summary>
      /// Defines whether FVL, SVC, DLCO, … test should be started (skip test type selection form)</summary>
      /// <remarks>supported since EWP version >= 1.8.0.6</remarks>
      public const string TestType  = "TestType";

      /// <summary>
      /// Planed/scheduled date (Alternative names: Expected Date/Time, Visit Date, Service date)</summary>
      /// <remarks>supported since EWP version >= 1.8.0.6</remarks>
      public const string Scheduled = "Scheduled";

      /// <summary>
      /// Who ordered this test</summary>
      /// <remarks>supported since EWP version >= 1.8.0.6</remarks>
      public const string OrderedBy = "OrderedBy";

      /// <summary>
      /// When the order was placed.</summary>
      /// <remarks>supported since EWP version >= 1.8.0.6</remarks>
      public const string Created   = "Created";
      
      /// <summary>
      /// Any comment that should be passed along.</summary>
      /// <remarks>supported since EWP version >= 1.8.0.6</remarks>
      public const string Comment   = "Comment";
      
      public const string OrderCustomList   = "OrderCustomList";
    }

    /// <summary>
    /// This command removes the order from the worklist. The action is only performed when the test hasn’t been executed yet.</summary>
    /// <remarks>supported since Version 1.8.0.14</remarks>
    public static class RemoveWorklist
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command   = "RemoveWorklist";

      /// <summary>
      /// EMR specific order identification. Has to be unique.</summary>
      public const string OrderID   = "OrderID";
    }

    /// <summary>
    /// Launches a test.</summary>
    public static class PerformTest
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command  = "PerformTest";

      /// <summary>
      /// EMR specific identification. Has to be unique.</summary>
      public const string OrderID  = "OrderID";

      /// <summary>
      /// Defines whether FVL, SVC, DLCO, … test should be started (skips the test type selection form)</summary>
      public const string TestType = "TestType";
    }

    /// <summary>
    /// Shows an existing test.</summary>
    public static class ShowTest
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command  = "ReviewTest";

      /// <summary>
      /// EMR specific identification. Has to be unique.</summary>
      public const string OrderID  = "OrderID";

      /// <summary>
      /// Time the test has been performed.</summary>
      public const string DateTime = "DateTime";
    }

    /// <summary>
    /// Opens the print preview or prints an existing test.</summary>
    public static class PrintTest
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command      = "PrintTest";

      /// <summary>
      /// EMR specific identification</summary>
      public const string OrderID      = "OrderID";

      /// <summary>
      /// Time the test has been performed</summary>
      public const string DateTime     = "DateTime";

      /// <summary>
      /// True: print preview is shown
      /// False: report is sent to printer.
      /// </summary>
      public const string ShowPreview  = "ShowPreview";

      /// <summary>
      /// Defines the layout to be used.
      /// BuiltIn layouts example:    BuildInReport::Summary
      /// customized layouts example: YourPersonalReportLayout.repx
      /// </summary>
      public const string ReportLayout = "ReportLayout";
    }

    /// <summary>
    /// Requests a list of patients fulfilling the search query.</summary>
    public static class SearchPatients
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command   = "SearchPatients";
      public const string FirstName = "FirstName";
      public const string LastName  = "LastName";
      public const string PatientID = "PatientID";
    }

    /// <summary>
    /// Result of search query.</summary>
    public static class SearchPatientsResult
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command = "SearchPatientsResult";
    }

    
    /// <summary>
    /// Requests test results.</summary>
    public static class GetTestResults
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command = "GetTestResults";

      /// <summary>
      /// Identifies the test of the given patient (needs to be exact to the second). </summary>
      public const string DateTime = "DateTime";

      #region -- not supported --//  public const string PatientID = "PatientID";
      //  /// <summary>
      //  /// Unique patient identification -> returns all test of that patient. 
      //  /// Additional filter can be set with DateBegin-DateEnd.</summary>
      //  public const string OrderID = "OrderID";
      //  /// <summary>
      //  /// EasyWarePro specific test identification. e.g. “SN:DT 200076:633404223171210000”</summary>
      //  public const string TestIdRef = "TestIdRef";
      //  public const string DateBegin = "DateBegin";
      //  /// <summary>
      //  /// Returns all test which are performed between DateBegin and DateEnd.</summary>
      //  public const string DateEnd = "DateEnd";
      #endregion
    }



    /// <summary>
    /// Returns test results (either when requested through GetTestResults or when Test has been performed).</summary>
    public static class TestResult
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command = "TestResult";

      /// <summary>
      /// Path to the attachment file (e.g. PDF). 
      /// (Attachment can be configured, see Configuration)</summary>
      public const string Attachment = "Attachment";
    }

    /// <summary>
    /// Indicates that error occurred.</summary>
    public static class Error
    {
      /// <summary>
      /// Command identification</summary>
      public const string Command = "Error";

      /// <summary>
      /// Error message</summary>
      public const string Message = "Message";

      /// <summary>
      /// More detailed error message (e.g. technical)</summary>
      public const string Details = "Details";
    }



  }





}
