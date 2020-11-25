import requests
import os
import json


# PART 1: Get sha256 for all files

url = 'https://www.virustotal.com/vtapi/v2/file/scan'

# provide api key 
params = {'apikey': 'ee152d817701866ceaa27f274dc871f84748c6f98109d6674b1f9dc16d878f3e'}

list_of_hashes = []

i = 0
# Find sha256 for test clean files
for file in os.listdir("./CLEAN"):
    if i == 100:
        break    
    if file.endswith(".pdf"):
        file_name = os.path.join("./CLEAN", file)
        files = {'file': (file_name, open(file_name, 'rb'))}
        # Send POST request to endpoint that returns JSON object containing sha256
        response = requests.post(url, files = files, params = params)
        list_of_hashes.append(response.json()['sha256'])
        i += 1
print(list_of_hashes)

# # Find sha256 for test dirty files
# for file in os.listdir("./MALWARE_PDF_PRE_04-2011_10982_files"):
#     if file.endswith(".pdf"):
#         file_name = os.path.join("./MALWARE_PDF_PRE_04-2011_10982_files", file)
#         files = {'file': (file_name, open(file_name, 'rb'))}
#         response = requests.post(url, files = files, params = params)
#         list_of_hashes.append(response.json()['sha256'])
#         i += 1
  
# PART 2: send sha256 to virusTotal API and analyze detection report

url = 'https://www.virustotal.com/vtapi/v2/file/report'
count_no_content = 0
for sha in list_of_hashes:
    # provide apikey
    params = {'apikey': 'ee152d817701866ceaa27f274dc871f84748c6f98109d6674b1f9dc16d878f3e', 'resource': str(sha)}
    response = requests.get(url, params = params)
    # if the file is detected in the system, then return report of the number of engines that successfully detected file
    if response.status_code == 200:
        print("******************************")
        print(response.json())
        total = response.json()['total']
        number_of_detections = response.json()['positives']
        
        # 76 engines in total 
        percent_detected = total/76 * 100
        precision = number_of_detections/total * 100
        recall = number_of_detections/76 * 100
        print("True Positive Rate: ", percent_detected)
        print("Pecision: ", precision)
        print("Recall: ", recall)
    # Status Code: 204 - no file detected
    else:
        count_no_content += 1
    
print("SUCCESS")
print("No Content: ", count_no_content)
