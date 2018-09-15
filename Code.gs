// BY: Akshaya Niraula
// ON: 2016 November 12th.
// AT: http://www.embedded-lab.com/.....

// modified by: MrMdudu
// on 15.09.2018
// for Arduino_Garden Project
 
// Steps are valid as of 2016 November 12th.
// 0) From Google spreadsheet, Tools &gt; Scriipt Editor...
// 1) Write your code
// 2) Save and give a meaningful name
// 3) Run and make sure "doGet" is selected
//    You can set a method from Run menu
// 4) When you run for the first time, it will ask 
//    for the permission. You must allow it.
//    Make sure everything is working as it should.
// 5) From Publish menu &gt; Deploy as Web App...
//    Select a new version everytime it's published
//    Type comments next to the version
//    Execute as: "Me (your email address)"
//    MUST: Select "Anyone, even anonymous" on "Who has access to this script"
//    For the first time it will give you some prompt(s), accept it.
//    You will need the given information (url) later. This doesn't change, ever!
 
// Saving the published URL helps for later.
// https://script.google.com/macros/s/---Your-Script-ID--Goes-Here---/exec?airhum=11&temp=12&rawlum=13&soilhum=14
// ?airhum=11&temp=12&rawlum=13&soilhum=14 added for testing 
 
// This method will be called first or hits first  
function doGet(e){
  Logger.log("--- doGet ---");
 
 var airhum = "",
     temp = "",
     rawlum = "",
     soilhum = "";
 
  try {
 
    // this helps during debuggin
    if (e == null){e={}; e.parameters = {airhum:"-1",temp:"-2",rawlum:"-3",soilhum:"-4"};}
 
    airhum = e.parameters.airhum;
    temp = e.parameters.temp;
    rawlum = e.parameters.rawlum;
    soilhum = e.parameters.soilhum;
 
    // save the data to spreadsheet
    save_data(airhum, temp, rawlum, soilhum);
 
 
    return ContentService.createTextOutput("Wrote:\n  airhum: " + airhum 
                                            + "\n  temp: " + temp 
                                            + "\n  rawlum: " + rawlum 
                                            + "\n  soilhum: " + soilhum);
 
  } catch(error) { 
    Logger.log(error);    
    return ContentService.createTextOutput("oops...." + error.message 
                                            + "\n" + new Date() 
                                            + "\nairhum: " + airhum
                                            + "\ntemp: " + temp
                                            + "\nrawlum: " + rawlum
                                            + "\n  soilhum: " + soilhum);
  }  
}
 
// Method to save given data to a sheet
function save_data(airhum, temp, rawlum, soilhum){
  Logger.log("--- save_data ---"); 
 
 
  try {
    var dateTime = new Date();
 
    // Paste the URL of the Google Sheets starting from https thru /edit
    // For e.g.: https://docs.google.com/..../edit 
    var ss = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/<--Your-Spread-Sheet-URL-->/edit");
    var summarySheet = ss.getSheetByName("Summary");
    var dataLoggerSheet = ss.getSheetByName("DataLogger"); 
 
    // Get last edited row from DataLogger sheet
    var row = dataLoggerSheet.getLastRow() + 1;

    // Start Populating the data
    dataLoggerSheet.getRange("A" + row).setValue(row -1); // ID
    dataLoggerSheet.getRange("B" + row).setValue(dateTime); // dateTime
    dataLoggerSheet.getRange("C" + row).setValue(airhum); // value
    dataLoggerSheet.getRange("D" + row).setValue(temp); // value
    dataLoggerSheet.getRange("E" + row).setValue(rawlum); // value
    dataLoggerSheet.getRange("F" + row).setValue(soilhum); // value
 
    // Update summary sheet
    summarySheet.getRange("B1").setValue(dateTime); // Last modified date
    // summarySheet.getRange("B2").setValue(row - 1); // Count 
  }
 
  catch(error) {
    Logger.log(JSON.stringify(error));
  }
 
  Logger.log("--- save_data end---"); 
}