#!/usr/bin/env python
#
# Copyright (c) 2014, Philippe Proulx <eepp.ca>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the pytoutv nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL Philippe Proulx BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import sys
from setuptools import setup
import qtibeeprogress


# Make sure we run Python 3.3+ here
v = sys.version_info
if v.major < 3 or v.minor < 3:
    sys.stderr.write('Sorry, qtibeeprogress needs Python 3.3+\n')
    sys.exit(1)

entry_points = {
    'gui_scripts': [
        'qtibeeprogress = qtibeeprogress.app:run'
    ],
}

packages = [
    'qtibeeprogress',
]

package_data = {
    'qtibeeprogress': [
        'dat/ui/*.ui',
    ]
}

install_requires = [
    'pyzmq>=14.0.0',
]

setup(name='qtibeeprogress',
      version=qtibeeprogress.__version__,
      description='GUI progress for tigerbeetle builder',
      author='Philippe Proulx',
      url='https://github.com/eepp/tigerbeetle',
      keywords='tigerbeetle',
      packages=packages,
      package_data=package_data,
      install_requires=install_requires,
      entry_points=entry_points)
