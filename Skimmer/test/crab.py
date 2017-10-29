from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'ctpps_run_monitor'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'RunMonitor.py'
#config.JobType.disableAutomaticOutputCollection = False
config.JobType.outputFiles = ['outputfile_304447.root']

#config.Data.inputDataset = '/SingleMuon/Run2017E-PromptReco-v1/RECO'
#config.Data.inputDataset = '/SingleElectron/Run2017E-PromptReco-v1/RECO'
config.Data.inputDataset = '/ZeroBias/Run2017E-PromptReco-v1/RECO'

config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 5000
config.Data.runRange = '304447'

config.Data.outLFNDirBase = '/store/group/dpg_ctpps/comm_ctpps/Timing_Commissioning/MonitorOffline'
config.Site.storageSite = 'T2_CH_CERN'
