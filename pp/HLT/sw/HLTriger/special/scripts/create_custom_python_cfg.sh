#!/bin/bash -
#command line: create_custom_cfg.sh ../../cfg/special/test/OnLine_HLT_Test.py ../../cfg/customHLT_HfActivity.py ../../cfg/addHLT_EcalActivity.py ../../cfg/addHLT_PixelActivity.py ../../custom_essource.py ../../mycustom_options.py
add_custom_cfg.sh ../../cfg/special/test/OnLine_HLT_Test.py ../../cfg/customHLT_HfActivity.py
add_custom_cfg.sh custom_OnLine_HLT_Test.py ../../cfg/addHLT_EcalActivity.py
add_custom_cfg.sh custom_OnLine_HLT_Test.py ../../cfg/addHLT_PixelActivity.py
add_custom_cfg.sh custom_OnLine_HLT_Test.py ../../custom_essource.py
add_custom_cfg.sh custom_OnLine_HLT_Test.py ../../mycustom_options.py
add_custom_cfg.sh custom_OnLine_HLT_Test.py /home/frankma/UserCode/SavedFMa/cmssw/MyConfiguration/Sequences/python/output_cfg_summary.py
