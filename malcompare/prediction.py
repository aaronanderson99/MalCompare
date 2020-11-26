import pandas as pd
import sys
import json

if len(sys.argv) < 2:
    print('Error: Invalid input for Tool Configuration File')

tool_config_file = sys.argv[1]


has_ground_truth = 0
ground_truth = None
if len(sys.argv) > 2:
    ground_truth_file = sys.argv[2]
    if ground_truth_file != 'no_ground_truth':
        has_ground_truth = 1
    else:
        ground_truth_file = None
        has_ground_truth = 0

has_query = 0
if len(sys.argv) > 3:
    query = sys.argv[3]
    if query != 'no_query':
        has_query = 1
    else:
        has_query = 0

with open(tool_config_file) as tool_config:
    tools = json.load(tool_config)

if has_ground_truth:
    ground_truth = pd.read_csv(ground_truth_file, sep = ":", index_col = 'file')
    ground_truth.columns = ['Ground_Truth']


def read_predictions():
    predictions_list = list()
    for t in tools:
        df = pd.read_csv(t['file'], sep = ":", index_col = 'file')
        df.columns = [t['name']]
        predictions_list.append(df)
    
    return pd.concat(predictions_list, axis=1, sort=True)

def prediction(x):
    malicious_probability = 1
    benign_probability = 1

    for t in tools:
        if x.get(t['name']) == 'malicious':
            malicious_probability *= (1 - t['false_negative_rate'])
            benign_probability *= t['false_positive_rate']
        elif x.get(t['name']) == 'benign':
            malicious_probability *= t['false_negative_rate']
            benign_probability *= (1 - t['false_positive_rate'])

    if benign_probability > malicious_probability:
        return 'benign'
    else:
        return 'malicious'


def summary(x, prediction, gt):
    num_malicious = 0
    num_benign = 0
    num_total = 0
    num_agreement = 0

    true_positive = 0
    true_negative = 0
    false_positive = 0
    false_negative = 0

    for r_x, r_p in zip(x, prediction):
        if r_x == 'malicious':
            num_malicious = num_malicious + 1
            num_total = num_total + 1
        elif r_x == 'benign':
            num_benign = num_benign + 1
            num_total = num_total + 1
        if r_x == r_p:
            num_agreement = num_agreement + 1

    percent_agreement = num_agreement / num_total

    if has_ground_truth:
        for r_x, r_a in zip(x, gt):
            if (r_x != 'malicious' and r_x != 'benign') or (r_a != 'malicious' and r_a != 'benign'):
                continue
            if r_x == 'malicious':
                if r_a == 'malicious':
                    true_positive = true_positive + 1
                elif r_a == 'benign':
                    false_positive = false_positive + 1
            elif r_x == 'benign':
                if r_a == 'benign':
                    true_negative = true_negative + 1
                elif r_a == 'malicious':
                    false_negative = false_negative + 1
        if false_positive == 0:
            false_positive_rate = 0.00
        else:
            false_positive_rate = false_positive / (false_positive + true_negative)
        if false_negative == 0:
            false_negative_rate = 0.00
        else:
            false_negative_rate = false_negative / (false_negative + true_positive)

        accuracy = (true_positive + true_negative) / (true_positive + true_negative + false_positive + false_negative)
        
        return pd.Series([num_benign, num_malicious, percent_agreement, accuracy, true_positive, true_negative, false_positive, false_negative, false_positive_rate, false_negative_rate])
    else:
        return pd.Series([num_benign, num_malicious, percent_agreement])

total_df = read_predictions()
total_df['Prediction'] = total_df.apply(prediction, axis=1)
if has_ground_truth:
    total_df = total_df.join(ground_truth['Ground_Truth'])


if has_ground_truth:
    summary_df = total_df.apply(summary, prediction=total_df['Prediction'], gt=total_df['Ground_Truth'], axis=0).transpose()
else:
    summary_df = total_df.apply(summary, prediction=total_df['Prediction'], gt=None, axis=0).transpose()

if has_ground_truth:
    summary_df.columns=['num_benign', 'num_malicious', 'prediction_agreement', 'accuracy', 'true_positives', 'true_negatives', 'false_positives', 'false_negatives', 'false_positive_rate', 'false_negative_rate']
else:
    summary_df.columns=['num_benign', 'num_malicious', 'prediction_agreement']


if has_query == 1:
    query_df = total_df.query(query)
    print(query_df)
else:
    print('Classification')
    print(total_df)
    print()
    print()
    print('Summary')
    print(summary_df)
    print()
    print()
