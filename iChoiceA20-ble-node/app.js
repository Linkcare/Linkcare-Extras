var noble = require('noble');
var prompt = require("prompt");
var Jetty = require("jetty");
var jetty = new Jetty(process.stdout);
var fs = require('fs');
var read = require('./node_modules/prompt/node_modules/read');

var buffer = new Buffer("AA5502B0B2", "hex");
var devices = {};
var ids = {};
var filePath = 'devices.txt';

function loadDevices(){
    var file = fs.readFileSync(filePath).toString().split("\n");
    for(var i in file) {
        var line = file[i];
        if (line === ""){
            continue
        }
        var split = line.split('\t');
        var device = split[0], id = split[1];
        ids[id] = true;
        devices[device] = id
    }
}

jetty.clear();


function askForID(prompt, callback){
    read({
        prompt: "",
        silent: false,
        default: "",
        input: process.stdin,
        output: false
    }, callback);

    jetty.moveTo([1,0]).erase(100);
    jetty.moveTo([1,0]).text(prompt);
}


loadDevices();


noble
    .on('stateChange', function(state) {
        jetty.moveTo([0,0]).erase(100);
        if (state === 'poweredOn') {
            jetty.moveTo([0,0]).text("Starting scanning...");
            noble.startScanning();
        } else {
            jetty.moveTo([0,0]).text("Stopping scanning...");
            noble.stopScanning();
        }
    })
    .on('scanStart', function() {
        jetty.moveTo([0,0]).erase(100);
        jetty.moveTo([0,0]).text("Scanning...");
    })
    .on('scanStop', function() {
        jetty.moveTo([0,0]).erase(100);
        jetty.moveTo([0,0]).text("Not scanning");
    })
    .on('connect', function() {
        jetty.moveTo([2,0]).erase(100);
        jetty.moveTo([2,0]).text("Connected");
    })
    .on('disconnect', function() {
        jetty.moveTo([2,0]).erase(100);
        jetty.moveTo([2,0]).text("Disconnected");
    });



function saveDevice(peripheral, id){
    jetty.moveTo([1,0]).erase(100);

    if(typeof ids[id] !== "undefined"){
        jetty.moveTo([1,0]).text("Error: ID already entered");
        return
    }
    ids[id] = true;
    devices[peripheral.address] = id;
    fs.appendFile(filePath, peripheral.address+"\t"+id+"\n", function (err) {
        if (err){
            return console.log(err);
        }
        jetty.moveTo([1,0]).text("Saved device. Mac address: "+peripheral.address+", ID: "+id);
    });

}

noble.on('discover', function(peripheral){
    jetty.moveTo([1,0]).erase(100);
    jetty.moveTo([1,0]).text("Scanning..." + peripheral.advertisement.localName + ": "+ peripheral.address);
    if(peripheral.advertisement.localName !== "iChoice" || typeof devices[peripheral.address] !== "undefined"){
        return
    }

    noble.stopScanning();
    jetty.moveTo([2,0]).erase(100);
    jetty.moveTo([2,0]).text("Connecting...");

    peripheral.connect(function(error){



        peripheral.discoverAllServicesAndCharacteristics(function(error, services, characteristics){
            for (var i = 0, len = characteristics.length; i < len; i++) {
                characteristic = characteristics[i];

                if(characteristic.uuid !== "cd20"){
                    continue;
                }
                characteristic.write(buffer);

                if(peripheral.address == "unknown"){
                    askForID("Address of device:", function (err, result) {
                        peripheral.address = result;

                        askForID("ID of device ("+peripheral.address+"):", function (err, result) {
                            saveDevice(peripheral, result);
                            peripheral.disconnect(function(error){
                                noble.startScanning();
                            })
                        });
                    });
                    return;
                }


                askForID("ID of device ("+peripheral.address+"):", function (err, result) {
                    saveDevice(peripheral, result);
                    peripheral.disconnect(function(error){
                        noble.startScanning();
                    })
                });
            }

        });
    });


});

