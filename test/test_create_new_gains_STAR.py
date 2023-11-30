#!/usr/bin/env python

import datetime as dt

import create_new_gains_STAR as cn


def test_get_mid_doy():
    dt1 = dt.date(2023, 9, 9)
    dt2 = dt.date(2023, 11, 9)
    expected = dt.date(2023, 10, 9).timetuple().tm_yday
    got = cn.get_mid_doy(dt1, dt2)
    print("test_get_mid_doy: exp = ", expected)
    print("test_get_mid_doy: got = ", got)
    assert(got == expected)


def test_get_gain_low():
    mid_doy = 180
    lat = 60
    band = 1
    eq = 22.5
    #expected = float('1.123281')
    expected = 1
    got = cn.get_gain(mid_doy, lat, band, eq)
    print("test_get_gain: exp = ", expected)
    print("test_get_gain: got = ", got)
    assert(got == expected)

def test_get_gain_norm():
    mid_doy = 180
    lat = 59
    band = 3
    eq = 21
    expected = 3
    got = cn.get_gain(mid_doy, lat, band, eq)
    print("test_get_gain: exp = ", expected)
    print("test_get_gain: got = ", got)
    assert(got == expected)

def test_get_gain_high():
    mid_doy = 180
    lat = -39
    band = 3
    eq = 21
    #expected = float('1.123281')
    expected = 4
    got = cn.get_gain(mid_doy, lat, band, eq)
    print("test_get_gain: exp = ", expected)
    print("test_get_gain: got = ", got)
    assert(got == expected)

def test_get_gain_dark():
    mid_doy = 180
    lat = -80
    band = 1
    eq = 22.5
    #expected = float('1.123281')
    expected = 4
    got = cn.get_gain(mid_doy, lat, band, eq)
    print("test_get_gain: exp = ", expected)
    print("test_get_gain: got = ", got)
    assert(got == expected)


def test_get_avg_lat():
    in_list = ['-20 00\'00.00"  80 28\'10.92"', '-20 00\'00.00"  78 41\'34.44"']
    expected = (80.4697 + 78.6929) / 2.0
    got = cn.get_avg_lat(in_list)
    print("test_get_avg_lat: exp = ", expected)
    print("test_get_avg_lat: got = ", got)
    assert(got == expected)


def test_date_field_to_date():
    in_str = '07/05/2023 00:00:00'
    expected = dt.date(2023, 7, 5)
    got = cn.date_field_to_date(in_str)
    print("test_date_field_to_date: exp = ", expected)
    print("test_date_field_to_date: got = ", got)
    assert(got == expected)
