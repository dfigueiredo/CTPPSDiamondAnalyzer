import os

#CTPPS Tests
#path_ = '/store/group/dpg_ctpps/comm_ctpps/HPTDC_calibration/MinidaqRun302086/'
#path_ = '/store/group/dpg_ctpps/comm_ctpps/Timing_Commissioning/ClockTestsRuns/303982/'

#Global
#path_ = '/store/t0streamer/Data/PhysicsCommissioning/000/303/641/'
#path_ = '/store/t0streamer/Data/Physics/000/303/942/'

#Minidaq
path_ = '/store/t0streamer/Minidaq/A/000/303/982/'

file_save = open("../python/AutoGenerate_cff.py",'w') 

asps = []
print>>file_save, "readFiles=["
for root, dirs, files in os.walk(r'/eos/cms'+path_):
    for file in files:
        if file.endswith('.dat'):
            asps.append(file)
            print>>file_save, "'"+path_ + file + "',"
print>>file_save, "]"
file_save.close()
