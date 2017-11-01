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
  'ZeroBias_E' : '/ZeroBias/Run2017E-PromptReco-v1/RECO',
  'ZeroBias_F' : '/ZeroBias/Run2017F-PromptReco-v1/RECO',
  'SingleMuon_E' : '/SingleMuon/Run2017E-PromptReco-v1/RECO',
  'SingleMuon_F' : '/SingleMuon/Run2017F-PromptReco-v1/RECO',
  'SingleElectron_E' : '/SingleElectron/Run2017E-PromptReco-v1/RECO',
  'SingleElectron_F' : '/SingleElectron/Run2017F-PromptReco-v1/RECO',
}

runnumber = '1' 
filesPerJob = 1

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
    config.JobType.pyCfgParams = ["Run="+runnumber, "Type=RECO"]
    submit(config)

# Run303819, Low PU
runnumber = '303819'
filesPerJob = 5000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304508
runnumber = '304508'
filesPerJob = 5000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304625
runnumber = '304625'
filesPerJob = 5000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304663
runnumber = '304663'
filesPerJob = 5000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run304671
runnumber = '304671'
filesPerJob = 5000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_E']
doSubmit(listOfSamples)

# Run305207
runnumber = '305207'
filesPerJob = 5000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_F']
doSubmit(listOfSamples)

# Run305208
runnumber = '305208'
filesPerJob = 5000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_F']
doSubmit(listOfSamples)

# Run305237
runnumber = '305237'
filesPerJob = 5000
name = '_Run' + runnumber
listOfSamples = ['ZeroBias_F']
doSubmit(listOfSamples)
