####################################################

# Boiler plate code for importing automation_control form parent directory :SCREAM: 
import sys
import os
current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
sys.path.append(parent)

######################################################

from urllib3 import proxy_from_url
import automation_control as ctrl

number = 29
campaign = f'ctrl_camp{number}'
workflow = f'crtl_workflow{number}'
data_period = 'run' # todo use correct dataperiod number 
template = '/afs/cern.ch/user/m/mobrzut/automation/environment_based_on_jenkins_script/pps_workflow_for_cmssw12/mobrzut_test_1/CMSSW_12_4_0/src/RecoPPS/RPixEfficiencyTools/automation/TempCrabConfigs/TempCrabConfigEAWorker.py'
dataset = '/EGamma/Run2018B-12Nov2019_UL2018-v2/AOD'
proxy = '/afs/cern.ch/user/m/mobrzut/public/jenkins_proxy/jenkins.pem'

def get_runs_range(data_period):
    """MOCKED"""
    return '317080'


ctrl.submit_task_to_crab(campaign, workflow, data_period, get_runs_range(data_period), template, dataset, proxy)