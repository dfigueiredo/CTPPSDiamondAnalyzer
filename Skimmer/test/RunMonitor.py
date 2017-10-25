import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import copy

####################
#    Input File    #
####################
data_type = "RECO"

if data_type=="RAW" or data_type == "raw" or data_type=="RECO" or data_type == "reco":
	PluginSource = "PoolSource"
if data_type =="DAT" or data_type == "dat":
	PluginSource = "NewEventStreamFileReader"

process = cms.Process("SkimmerCTPPS")

#########################
#    General options    #
#########################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options   = cms.untracked.PSet(
	#SkipEvent = cms.untracked.vstring('ProductNotFound')
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

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
    RunNumber = cms.untracked.uint32(304447),
    # If ufirstHisto == ulastHisto or ( ufirstHisto < 0 || ulastHisto < 0); fit maximum peak from 0 to 125 ns.
    ufirstHisto = cms.double(0), # min X histo, (fit and plot draw). 
    ulastHisto = cms.double(25), # max X histo, (fit and plot draw).
    reducedPlots_ = cms.untracked.uint32(0) # > 0, generates image plots automatically. #If 2, produce extra plots leading/trailing vs ToT per channel.
)

if data_type == "RECO":
    process.p = cms.Path(
        process.Monitor
    )
else:
    process.p = cms.Path(
        process.ctppsRawToDigi *
        process.recoCTPPS *
        process.ctppsDiamondRawToDigi *
        process.ctppsDiamondRecHits *
        process.ctppsDiamondLocalTracks *
        process.goodOfflinePrimaryVertices *
        process.Monitor
    )

########################
#      DEBUG CODE      #
########################
#print process.dumpPython()
