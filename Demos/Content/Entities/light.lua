-- Copyright 2014-2015 Jason R. Wendlandt
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
-- http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

-- component data table
components = {
	root = {
		type = "mesh",
		model = "Models/cube.am",
		transform = {
			position = { x = 0.0, y = 0.0, z = 0.0 },
			rotation = { x = 0.0, y = 0.0, z = 0.0 },
			scale = { x = 0.25, y = 0.25, z = 0.25 }
		},
        light_emitter = true,
        color = { r = 1.0, g = 1.0, b = 0.5, a = 1.0 },
	}
}
