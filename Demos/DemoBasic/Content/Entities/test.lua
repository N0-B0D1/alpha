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
        -- by setting the root node like this, and having all other
        -- scene components exist as children of the root node, we
        -- can set the root nodes transform, and that will become the
        -- Entities transform in the world.
		type = "primitive",
		transform = {
			position = { x = 0.0, y = 0.0, z = 0.0 },
			rotation = { x = 0.0, y = 0.0, z = 0.0 },
			scale = { x = 1.0, y = 1.0, z = 1.0 }
		},
		components = {
			-- components can exist inside other components
			primitive_internal1 = {
				type = "primitive",
				transform = {
					position = { x = 4.0, y = 0.0, z = 0.0 },
					rotation = { x = 0.0, y = 0.0, z = 0.0 },
					scale = { x = 1.0, y = 1.5, z = 1.0 }
				},
			},
			primitive_internal2 = {
				type = "primitive",
				transform = {
					position = { x = -4.0, y = 0.0, z = 0.0 },
					rotation = { x = 0.0, y = 0.0, z = 0.0 },
					scale = { x = 1.0, y = 0.5, z = 1.0 }
				},
			}
		}
	}
}
