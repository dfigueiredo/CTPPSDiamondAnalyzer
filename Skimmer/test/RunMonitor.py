import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import os,sys, atexit, copy

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.register('Run', 1, VarParsing.multiplicity.singleton, VarParsing.varType.int,"Run Number.")
options.register('Type','RECO', VarParsing.multiplicity.singleton, VarParsing.varType.string,"RECO, RAW or DAT format file.")
options.parseArguments()

print ("Options Enabled:")
print ("Type: %s" % options.Type)
print ("Run: %s" % options.Run)

####################
#    Input File    #
####################
data_type = options.Type

if data_type=="RAW" or data_type == "raw" or data_type=="RECO" or data_type == "reco":
	PluginSource = "PoolSource"
elif data_type =="DAT" or data_type == "dat":
	PluginSource = "NewEventStreamFileReader"
else:
  print("")
  print("") 
  raise RuntimeError, "Unknown option. EXIT! YOU NEED TO SETUP WITH ONE OF THE CORRECT OPTIONS."
  print("")


process = cms.Process("SkimmerCTPPS")

#########################
#    General options    #
#########################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options   = cms.untracked.PSet(
	#SkipEvent = cms.untracked.vstring('ProductNotFound')
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50000) )

#########################
#    RAW-DIGI-RECO      #
#########################
process.totemDAQMappingESSourceXML_TimingDiamond = cms.ESSource("TotemDAQMappingESSourceXML",
  verbosity = cms.untracked.uint32(0),
  subSystem = cms.untracked.string("TimingDiamond"),
  configuration = cms.VPSet(
    # before diamonds inserted in DAQ
    cms.PSet(
      validityRange = cms.EventRange("1:min - 283819:max"),
      mappingFileNames = cms.vstring(),
      maskFileNames = cms.vstring()
    ),
    # after diamonds inserted in DAQ
    cms.PSet(
      validityRange = cms.EventRange("283820:min - 999999999:max"),
      mappingFileNames = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/mapping_timing_diamond_2017.xml"),
      maskFileNames = cms.vstring()
    )
  )
)

# raw-to-digi conversion
process.load("EventFilter.CTPPSRawToDigi.ctppsRawToDigi_cff")

# local RP reconstruction chain with standard settings
process.load("RecoCTPPS.Configuration.recoCTPPS_cff")

# rechits production
process.load('Geometry.VeryForwardGeometry.geometryRP_cfi')
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondRecHits_cfi')

# local tracks fitter
process.load('RecoCTPPS.TotemRPLocal.ctppsDiamondLocalTracks_cfi')

#########################
#      Input files      #
#########################
from CTPPSDiamondAnalyzer.Skimmer.AutoGenerate_cff import readFiles
process.source = cms.Source (PluginSource, fileNames = cms.untracked.vstring(readFiles))

######################
#      Analyzer      #
######################
process.Monitor = cms.EDAnalyzer("CTPPSMonitor",
    tagType = cms.untracked.string(data_type),
    tagStatus = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagDigi = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagFEDInfo = cms.InputTag("ctppsDiamondRawToDigi", "TimingDiamond"),
    tagDiamondRecHits = cms.InputTag("ctppsDiamondRecHits"),
    tagDiamondLocalTracks = cms.InputTag("ctppsDiamondLocalTracks"),
    tagLocalTrack = cms.InputTag("totemRPLocalTrackFitter"),
    tagVertexCollection = cms.InputTag("offlinePrimaryVertices"),
    bx = cms.untracked.vint32(), #empty vector: no BX selection
    verbosity = cms.untracked.uint32(0),
    RunNumber = cms.untracked.uint32(options.Run),
    # If ufirstHisto == ulastHisto or ( ufirstHisto < 0 || ulastHisto < 0); fit maximum peak from 0 to 125 ns.
    ufirstHisto = cms.double(0), # min X histo, (fit and plot draw). 
    ulastHisto = cms.double(25), # max X histo, (fit and plot draw).
    reducedPlots = cms.untracked.uint32(0) # > 0, generates image plots automatically. #If 2, produce extra plots leading/trailing vs ToT per channel.
)

if data_type == "RECO" or data_type == "reco":
    process.p = cms.Path(
        process.Monitor
    )
else:
    process.p = cms.Path(
        process.ctppsDiamondRawToDigi *
        process.ctppsDiamondRecHits *
        process.ctppsDiamondLocalTracks *
        process.Monitor
    )

########################
#      DEBUG CODE      #
########################
#print process.dumpPython()
