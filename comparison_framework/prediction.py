import pandas as pd
import sys
import json

if len(sys.argv) < 2:
    print('Error: Invalid input for Tool Configuration File')

if len(sys.argv) > 2:
    ground_truth_file = sys.argv[2]
    has_ground_truth = 1
else:
    ground_truth_file = None
    has_ground_truth = 0

ground_truth = None
tool_config_file = sys.argv[1]

with open(tool_config_file) as tool_config:
    tools = json.load(tool_config)

if has_ground_truth:
    ground_truth = pd.read_csv(ground_truth_file, sep = ":", index_col = 'file')
    ground_truth.columns = ['Ground Truth']


def read_predictions():
    predictions_list = list()
    for t in tools:
        df = pd.read_csv(t['file'], sep = ":", index_col = 'file')
        df.columns = [t['name']]
        predictions_list.append(df)
    
    return pd.concat(predictions_list, axis=1)

def prediction(x):
    malicious_probability = 1
    benign_probability = 1

    for t in tools:
        if x.get(t['name']) == 'malicious':
            malicious_probability *= (1 - t['false_positive_rate'])
            benign_probability *= t['false_positive_rate']
        else:
            malicious_probability *= t['false_negative_rate']
            benign_probability *= (1 - t['false_negative_rate'])

    if benign_probability > malicious_probability:
        return 'benign'
    else:
        return 'malicious'


def summary(x, prediction):
    num_malicious = 0
    num_benign = 0
    num_total = 0
    num_agreement = 0

    true_positive = 0
    true_negative = 0
    false_positive = 0
    false_negative = 0

    for r_x, r_p in zip(x, prediction):
        num_total = num_total + 1
        if r_x == 'malicious':
            num_malicious = num_malicious + 1
        else:
            num_benign = num_benign + 1
        if r_x == r_p:
            num_agreement = num_agreement + 1

    percent_agreement = num_agreement / num_total

    if has_ground_truth:
        for r_x, r_a in zip(x, ground_truth['Ground Truth']):
            if r_x == 'malicious':
                if r_a == 'malicious':
                    true_positive = true_positive + 1
                else:
                    false_positive = false_positive + 1
            else:
                if r_a == 'benign':
                    true_negative = true_negative + 1
                else:
                    false_negative = false_negative + 1
        if false_positive == 0:
            false_positive_rate = 0.00
        else:
            false_positive_rate = false_positive / (false_positive + true_positive)
        if false_negative == 0:
            false_negative_rate = 0.00
        else:
            false_negative_rate = false_negative / (false_negative + true_negative)

        accuracy = (true_positive + true_negative) / (true_positive + true_negative + false_positive + false_negative)
        
        return pd.Series([num_benign, num_malicious, percent_agreement, accuracy, true_positive, true_negative, false_positive, false_negative, false_positive_rate, false_negative_rate])
    else:
        return pd.Series([num_benign, num_malicious, percent_agreement])

total_df = read_predictions()
total_df['Prediction'] = total_df.apply(prediction, axis=1)
if has_ground_truth:
    total_df['Ground Truth'] = ground_truth['Ground Truth']



summary_df = total_df.apply(summary, prediction=total_df['Prediction'], axis=0).transpose()

if has_ground_truth:
    summary_df.columns=['num_benign', 'num_malicious', 'prediction_agreement', 'accuracy', 'true_positives', 'true_negatives', 'false_positives', 'false_negatives', 'false_positive_rate', 'false_negative_rate']
else:
    summary_df.columns=['num_benign', 'num_malicious', 'prediction_agreement']

print('Classification')
print(total_df)
print()
print()
print('Summary')
print(summary_df)
