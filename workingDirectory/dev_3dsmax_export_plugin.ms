max modify mode
rollout expmesh "Export Mesh" width:377 height:67
(
	edittext edt_path "Path" pos:[10,10] width:300 height:17
	button btn_browse "Browse" pos:[320,8] width:50 height:21
	button btn_exp "Export" pos:[320,35] width:50 height:21
	checkbox chk_t "TexCoord" pos:[172,35] width:70 height:15 checked:true
	checkbox chk_n "Normals" pos:[244,35] width:62 height:15 checked:true
	on expmesh open do
	(
	  edt_path.text = (GetDir #Export) + "\\model.dfm"
	)
	on btn_browse pressed do
	(
	  s = GetSaveFileName types:"DevFileModel (*.dfm)|*.dfm|All files (*.*)|*.*"
	if (s != undefined) do
	    edt_path.text = s
	)
	on btn_exp pressed do
	(
	local bT=0
	local bN=0
	
	if (chk_n.checked == true) then bN=1
	if (chk_t.checked == true) then bT=1
	
	if edt_path.text != undefined do
	(
		file = fopen edt_path.text "wb"
		
		writeLong file 0xF1A2B400 --id max file
		
		mcount = 0
		for obj in Objects do
			if (classOf obj != BoneGeometry) and (classOf obj != Biped_Object) and (canConvertTo obj TriMeshGeometry) then
				mcount += 1
		
		writeLong file mcount
		
		vt = 1
		if (bT == 1) and (bN != 1) then
			vt = 2;
		else if (bN == 1) and (bT != 1) then
			vt = 3;
		else if (bN == 1) and (bN == 1) then
			vt = 4;
		
		writeLong file vt
		
		for obj in Objects do
			if (classOf obj != BoneGeometry) and (classOf obj != Biped_Object) and (canConvertTo obj TriMeshGeometry) then
			(
				mesh = copy obj
				writeLong file (obj.name.count + 1)
				writeString file obj.name
				convertToMesh mesh
				
				writeLong file 0xE1A2B401  				--id vertex mesh
				writeLong file (mesh.numFaces * 3)	--num vertices
				writeLong file 0 				 				--num indexes
				
				bbMin = obj.min;
				bbMax = obj.max;
				(
					writeFloat file bbMax.x
					writeFloat file bbMax.z
					writeFloat file bbMax.y
				)
				(
					writeFloat file bbMin.x
					writeFloat file bbMin.z
					writeFloat file bbMin.y
				)
				
				for i = 1 to mesh.numFaces do
				(
					f = (getFace mesh i)
					if (mesh.numTVerts > 0) then
						tvf = (getTVFace mesh i)
					
					n=meshop.GetFaceRNormals mesh i
					v = getVert mesh f.x
					writeFloat file v.x
					writeFloat file v.z
					writeFloat file v.y
					
					if (bN == 1) then
					(
						writeFloat file n[1].x
						writeFloat file n[1].z
						writeFloat file n[1].y
					)
					
					if (bT == 1) then
					(
						if (mesh.numTVerts > 0) then
						(
							t = (getTVert mesh tvf.x)
							writeFloat file t.x
							writeFloat file (1-t.y)
						)
						else 
						(
							writeFloat file 0
							writeFloat file 0
						)
					)
					
					v = getvert mesh f.z
					writeFloat file v.x
					writeFloat file v.z
					writeFloat file v.y
					
					if (bN == 1) then
					( 
						writeFloat file n[3].x
						writeFloat file n[3].z
						writeFloat file n[3].y
					)
					
					if (bT == 1) then
					(
						if (mesh.numTVerts > 0) then
						(
							t = (getTVert mesh tvf.z)
							writeFloat file t.x
							writeFloat file (1-t.y)
						)
						else 
						(
							writeFloat file 0
							writeFloat file 0
						)
					)
					
					v = getvert mesh f.y
					writeFloat file v.x
					writeFloat file v.z
					writeFloat file v.y
					
					if (bN == 1) then
					(
						writeFloat file n[2].x
						writeFloat file n[2].z
						writeFloat file n[2].y
					)
					
					if (bT == 1) then
					(
						if (mesh.numTVerts > 0) then
						(
							t = (getTVert mesh tvf.y)
							writeFloat file t.x
							writeFloat file (1-t.y)
						)
						else 
						(
							writeFloat file 0
							writeFloat file 0
						)
					)
				)
			
			delete mesh
			)
		fclose file
		)
	)
) m_roll = newrolloutfloater "Dev File Exporter" 400 93

addrollout expmesh m_roll