#!/usr/bin/env python

##############
# Multi CRAB #
##############

import time 

if __name__ == '__main__':
  from CRABAPI.RawCommand import crabCommand

def submit(config):
  res = crabCommand('submit', config = config)

from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

dataset = {
  'ZeroBias_Cv2' : '/ZeroBias/Run2017C-PromptReco-v2/RECO',
  'ZeroBias_Cv3' : '/ZeroBias/Run2017C-PromptReco-v3/RECO',
  'ZeroBias_D' : '/ZeroBias/Run2017D-PromptReco-v1/RECO',
  'ZeroBias_E' : '/ZeroBias/Run2017E-PromptReco-v1/RECO',
  'ZeroBias_F' : '/ZeroBias/Run2017F-PromptReco-v1/RECO',
  'SingleMuon_E' : '/SingleMuon/Run2017E-PromptReco-v1/RECO',
  'SingleMuon_F' : '/SingleMuon/Run2017F-PromptReco-v1/RECO',
  'SingleElectron_E' : '/SingleElectron/Run2017E-PromptReco-v1/RECO',
  'SingleElectron_F' : '/SingleElectron/Run2017F-PromptReco-v1/RECO',
}

runnumber = '1' 
filesPerJob = 1
ufirst = '0.'
ulast = '25.'

config.General.transferLogs = True
config.General.transferOutputs = True
config.JobType.pluginName = 'Analysis'
config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.publication = False
config.Site.storageSite = 'T2_CH_CERN'
config.JobType.psetName = 'RunMonitor.py'
#config.Data.ignoreLocality = True 
#config.JobType.disableAutomaticOutputCollection = False

def doSubmit(listOfSamples):
  for sample in listOfSamples:
    newName = sample+name
    config.JobType.outputFiles = ['outputfile_'+runnumber+'.root']
    config.General.workArea = 'crab_'+ newName
    config.General.requestName = sample
    config.Data.inputDataset = dataset[sample]
    config.Data.unitsPerJob = filesPerJob
    config.Data.runRange = runnumber
    config.Data.outputDatasetTag = sample
    config.Data.outLFNDirBase = '/store/group/dpg_ctpps/comm_ctpps/Timing_Commissioning/MonitorOffline/' + newName
    config.Site.storageSite = 'T2_CH_CERN'
    config.JobType.pyCfgParams = ["Run="+runnumber, "Type=RECO", "uFirst="+ufirst, "uLast="+ulast]
    submit(config)

# Run300459
runnumber = '300459'
ufirst = '50'
ulast = '90'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_Cv2']
doSubmit(listOfSamples)

# Run300461, fill 6046
runnumber = '300461'
ufirst = '50'
ulast = '90'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_Cv2']
doSubmit(listOfSamples)

# Run300462, fill 6046
runnumber = '300462'
ufirst = '50'
ulast = '90'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_Cv2']
doSubmit(listOfSamples)

# Run300463, fill 6046
runnumber = '300463'
ufirst = '50'
ulast = '90'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_Cv2']
doSubmit(listOfSamples)

# Run300464, fill 6046
runnumber = '300464'
ufirst = '50'
ulast = '90'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_Cv2']
doSubmit(listOfSamples)

# Run300466, fill 6046
runnumber = '300466'
ufirst = '50'
ulast = '90'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_Cv2']
doSubmit(listOfSamples)

# Run300467, fill 6046
runnumber = '300467'
ufirst = '50'
ulast = '90'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_Cv2']
doSubmit(listOfSamples)

# Run301998
runnumber = '301998'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_Cv3']
doSubmit(listOfSamples)

# Run302225
runnumber = '302225'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_D']
doSubmit(listOfSamples)

# Run302240
runnumber = '302240'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_D']
doSubmit(listOfSamples)

# Run302278
runnumber = '302278'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_D']
doSubmit(listOfSamples)

# Run302448
runnumber = '302448'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_D']
doSubmit(listOfSamples)

# Run302597
runnumber = '302597'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_D']
doSubmit(listOfSamples)

# Run303819, Low PU
runnumber = '303819'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run303832
runnumber = '302832'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run303838
runnumber = '303838'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304169
runnumber = '304169'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304447
runnumber = '304447'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304508
runnumber = '304508'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304625
runnumber = '304625'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304663
runnumber = '304663'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304671
runnumber = '304671'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run305207
runnumber = '305207'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_F']
doSubmit(listOfSamples)

# Run305208
runnumber = '305208'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_F']
doSubmit(listOfSamples)

# Run305237
runnumber = '305237'
ufirst = '0'
ulast = '25'
filesPerJob = 10000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_F']
doSubmit(listOfSamples)
