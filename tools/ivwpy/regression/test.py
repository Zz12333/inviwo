#*********************************************************************************
#
# Inviwo - Interactive Visualization Workshop
#
# Copyright (c) 2013-2015 Inviwo Foundation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
#*********************************************************************************

import glob
import os

from .. util import *

class Test:
	def __init__(self, name, group, path):
		self.group = group
		self.path = path
		self.name = name
		self.script = ""

	def __str__(self):
		return self.toString()

	def toString(self):
		return self.group + "/" + self.name

	def getWorkspaces(self):
		paths = glob.glob(self.path +"/*.inv")
		return paths

	def getImages(self):
		imgs = glob.glob(self.path +"/*.png")
		imgs = [os.path.relpath(x, self.path) for x in imgs]
		return imgs
		
	def report(self, report):
		report['group'] = self.group
		report['name'] = self.name
		report['path'] = self.path
		report['script'] = self.script
		return report

	def makeOutputDir(self, base):
		if not os.path.isdir(base):
			raise RegressionError("Output dir does not exsist: " + dir)

		mkdir([base, self.group])
		mkdir([base, self.group, self.name])
		return toPath([base, self.group, self.name])

		raise RegressionError("Invalid Test kind")