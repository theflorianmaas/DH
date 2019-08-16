#!/usr/bin/env python3
"""
This is an example of how the pytradfri-library can be used async.

To run the script, do the following:
$ pip3 install pytradfri
$ Download this file (example_async.py)
$ python3 example_async.py <IP>

Where <IP> is the address to your IKEA gateway. The first time
running you will be asked to input the 'Security Code' found on
the back of your IKEA gateway.
"""

# Hack to allow relative import above top level package
import sys
import os
folder = os.path.dirname(os.path.abspath(__file__))  # noqa
sys.path.insert(0, os.path.normpath("%s/.." % folder))  # noqa

from pytradfri import Gateway
from pytradfri.api.aiocoap_api import APIFactory
from pytradfri.error import PytradfriError
from pytradfri.util import load_json, save_json

import asyncio
import uuid
import argparse
import json

lightArrayId = []
lightArraySts = []
lightArrayColor = []
lightArrayValue = []

groupArrayId = []
groupArraySts = []
groupArrayValue = []
groupArrayMood = []

print("start")


CONFIG_FILE = 'tradfri_standalone_psk.conf'

parser = argparse.ArgumentParser()
parser.add_argument('host', metavar='IP', type=str,
                    help='IP Address of your Tradfri gateway')
parser.add_argument('-K', '--key', dest='key', required=False,
                    help='Key found on your Tradfri gateway')
args = parser.parse_args()


if args.host not in load_json(CONFIG_FILE) and args.key is None:
    print("Please provide the 'Security Code' on the back of your "
          "Tradfri gateway:", end=" ")
    key = input().strip()
    if len(key) != 16:
        raise PytradfriError("Invalid 'Security Code' provided.")
    else:
        args.key = key
print("start2")

#try:
    # pylint: disable=ungrouped-imports
#    from asyncio import ensure_future
#except ImportError:
    # Python 3.4.3 and earlier has this as async
    # pylint: disable=unused-import
#    from asyncio import async
#    ensure_future = async



def run():
    print("start3")
    # Assign configuration variables.
    # The configuration check takes care they are present.
    conf = load_json(CONFIG_FILE)

    try:
        identity = conf[args.host].get('identity')
        psk = conf[args.host].get('key')
        api_factory = APIFactory(host=args.host, psk_id=identity, psk=psk)
    except KeyError:
        identity = uuid.uuid4().hex
        api_factory = APIFactory(host=args.host, psk_id=identity)

        try:
            psk = api_factory.generate_psk(args.key)
            print('Generated PSK: ', psk)

            conf[args.host] = {'identity': identity,
                               'key': psk}
            save_json(CONFIG_FILE, conf)
        except AttributeError:
            raise PytradfriError("Please provide the 'Security Code' on the "
                                 "back of your Tradfri gateway using the "
                                 "-K flag.")

    api = api_factory.request

    gateway = Gateway()
    print("inizio")
  
    devices_command = gateway.get_moods()
    devices_commands = api(devices_command)
    moods = api(devices_commands)

    print(moods)
    for mood in moods:
        print(mood.path)
		
run()
