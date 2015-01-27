
-- component data table
components = {
	-- transform is an invalid component type, and should raise a warning in the log file.
    transform0 = {
		type = "transform",
		x = 10.5,
		y = -1,
		z = -20
	},
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
			primitive_internal = {
				type = "primitive",
				transform = {
					position = { x = 0.0, y = 0.0, z = 0.0 },
					rotation = { x = 0.0, y = 0.0, z = 0.0 },
					scale = { x = 1.0, y = 1.0, z = 1.0 }
				},
			}
		}
	}
}
