# For Data Trigger
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
    'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/MuOnia/AOD/12Oct2013-v1/20000/0045B338-934C-E311-BEF1-0026189438B0.root'     
    )
)

goodJSON = 'Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)

process.TFileService = cms.Service ('TFileService',
    fileName = cms.string (
    'TagAndProbe2019.root'
    )
)
  
process.demo = cms.EDAnalyzer('TagAndProbe2019',
	verbose = cms.bool(False),
	triggerflag = cms.bool(True), # True = Data and False = MC		      
	# Trigger
	TriggerResultsTag = cms.untracked.InputTag("TriggerResults", "", "HLT"),
	TriggerEventTag = cms.untracked.InputTag("hltTriggerSummaryAOD", "", "HLT"),
	#PathName = cms.untracked.vstring("HLT_DoubleMu7_v"),
	PathName = cms.untracked.vstring(["HLT_Dimuon10_Jpsi_Barrel_v","HLT_Dimuon7_PsiPrime_v"]),
	# RECO Labels
	primaryVertexProducer = cms.InputTag("offlinePrimaryVertices"),
	recoMuonsLabel = cms.InputTag("muons"), 
	# RECO Configs
	minMuPt = cms.double(2.0),# in GeV
	maxMuEta = cms.double(2.4),
	minResonanceMass = cms.double(2.8),# in GeV
	maxResonanceMass = cms.double(3.4)# in GeV 
)


process.p = cms.Path(process.demo)
