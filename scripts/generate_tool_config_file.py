import sys
import json

tool_config_file = sys.argv[1]

tools = [
    {'name': 'PJScan', 'file': 'pjscan_output.txt', 'false_positive_rate': 1.00, 'false_negative_rate': 1.00},
    {'name': 'PDFRate', 'file': 'pdfrate_output.txt', 'false_positive_rate': 0.05, 'false_negative_rate': 0.05},
    {'name': 'Slayer', 'file': 'slayer_output.txt', 'false_positive_rate': 0.05, 'false_negative_rate': 0.05},
    {'name': 'Hidost', 'file': 'hidost_output.txt', 'false_positive_rate': 0.05, 'false_negative_rate': 0.05},
]

with open(tool_config_file, 'w') as outfile:
    json.dump(tools, outfile, indent=4)