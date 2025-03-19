#!/usr/bin/env python

import sys
import csv

then_file = 'GLIMS_STARs_Raup_20231130.csv'
now_file  = 'GLIMS_STARs_Raup_Kargel_2025-03-17.csv'

gains_start_col = 17
gs = gains_start_col

with open (then_file, 'r') as then:
    then_reader = csv.reader(then, delimiter='|', quotechar='^')

    with open (now_file, 'r') as now:
        now_reader = csv.reader(now, delimiter='|', quotechar='^')

        for row_then in then_reader:
            row_now  = next(now_reader)

            if row_then[gs] == row_now[gs] and row_then[gs+1] == row_now[gs+1] and row_then[gs+2] == row_now[gs+2]:
               continue

            else:
                print(f'Line {then_reader.line_num}:  V1: {row_then[gs]} -> {row_now[gs]}; V2: {row_then[gs+1]} -> {row_now[gs+1]}; V3: {row_then[gs+2]} -> {row_now[gs+2]}')
