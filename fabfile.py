#!/usr/bin/env python

from fabric.api import *

env.arduino_model = 'pro328'

def clean():
    local('ino clean')

def build():
    local('ino build -m %(arduino_model)s' % env)

def upload():
    local('ino upload -m %(arduino_model)s' % env)

def serial():
    local('ino serial')

