#!/usr/bin/env python

import sys
import datetime as dt
import re
import csv
import urllib.request
from subprocess import run

from tqdm import tqdm


def gain_col(band=1):
    # column numbers are zero-based.
    # Band numbers are one-based.
    # VNIR1 is at col 17
    return 16 + band


def get_mid_doy(dt1, dt2):
    mid_date = dt1 + (dt2-dt1)/2
    doy = mid_date.timetuple().tm_yday
    return doy


def get_gain(mid_doy, lat, band, eq_crossing_time):
    url = f'https://www.glims.org/cgi-bin/aster_gain.pl?doy={mid_doy}&lat={lat}&band={band}&eq_time={eq_crossing_time}'

    #print("Querying {}".format(url))
    try:
        with urllib.request.urlopen(url) as response:
            gain_response = response.read()
    except:
        print("Couldn't get information from", url)
        sys.exit(1)

    # Return value should look like:
    '''
    ASTER gains:
    Key: For DOY=180, Lat=39, Band=3, Eq_cross_time=21: Gain = 2.982356 (norm gain at 98.24% saturation)
    '''

    # 0.x = dark
    # 1.x = high
    # 2.x = normal
    # 3.x = low1
    # 4.x = low2
    # 5.x = bright

    gain_str = re.findall('Gain = +[\d.-]+', str(gain_response), re.MULTILINE)
    gain = float(gain_str[0].split('=')[1].strip())

    # Calculated integerized gain.  Adjusting for saturation problem by
    # adding a constant fraction (toward lower gain).

    saturation_adjustment = 0.2
    int_gain = int(gain + saturation_adjustment)
    # Map these gains to 1=low, 3=normal, 4=high
    
    gain_mapping = {0: 4,
                    1: 4,
                    2: 3,
                    3: 1,
                    4: 1,
                    5: 1,
                   }

    return gain_mapping[int_gain]


def get_avg_lat(points):
    '''
    Input is list of text-formatted points, each of which looks like
    -139 47'49.19"  61 17'07.08"
    or the empty string
    '''
    lats = []       # list of floats

    for p in points:
        if p == '':
            break
        (lon_deg, lon_ms, lat_deg, lat_ms) = p.split()
        lat_min, lat_sec = lat_ms.split("'")
        lat_sec = lat_sec.strip('"')
        lat = float(lat_deg) + float(lat_min)/60.0 + float(lat_sec)/3600.0
        lats.append(lat)

    avg_lat = sum(lats)/len(lats)
    return avg_lat


def date_field_to_date(datetime_str):
    dt_str, time_str = datetime_str.split(' ')
    (m, d, y) = dt_str.split('/')
    dt1 = dt.date(int(y), int(m), int(d))
    return dt1


def wc(filename):
    return int(run(['wc', '-l', filename], capture_output=True).stdout.split()[0])


def main():

    today = dt.date.today().isoformat()

    infile = 'GLIMS_STARs_01Jan23-31Dec23(STARTool)_Final.csv'
    outfile = f'GLIMS_STARs_Raup_Kargel_{today}.csv'

    current_eq_crossing_time = 21.667

    dt1_col = 38
    dt2_col = 39
    points_start_col = 49
    gains_start_col = 17

    numlines = wc(infile)
    print(f'{infile} has {numlines} lines.')

    with open(infile , 'r') as in_fh:
        in_reader = csv.reader(in_fh, delimiter='|', quotechar='^')

        with open(outfile, 'w') as out_fh:
            out_writer = csv.writer(out_fh, delimiter='|', quotechar='^')

            for row in tqdm(in_reader, desc='Looping through rows: ', total=numlines, ascii=True):
                if 'Lifetime' in row[dt1_col]:
                    out_writer.writerow(row)
                    continue
                dt1 = date_field_to_date(row[dt1_col])
                dt2 = date_field_to_date(row[dt2_col])
                mid_doy = get_mid_doy(dt1, dt2)
                lat = get_avg_lat(row[points_start_col:])
                new_gains = []
                for band in (1,2,3):
                    new_gains.append(get_gain(mid_doy, lat, band, current_eq_crossing_time))
                new_row = row.copy()
                new_row[gains_start_col:gains_start_col+len(new_gains)] = new_gains
                out_writer.writerow(new_row)


if __name__ == '__main__':
    main()
