// Copyright (C) 2025 Jiri Fajta
// License: MIT license

/* Snipped code:
	This code cannot be used to run, but it provides an idea how to use primitivemapping(...).
	PSYQ GTE function are just used as illustration for easier reading in this snipped code in contrast to PSn00bSDK.
*/

//PROTOTYPE
void process_polygon4(Mesh *mesh, POLY_X *poly);
void process_polygon3(Mesh *mesh, POLY_X *poly);
void process_polygon2(Mesh *mesh, POLY_X *poly);
void process_polygon1(Mesh *mesh, POLY_X *poly);

void process_polygon(Mesh *mesh){
	POLY_X *poly = (POLY_X*)0x1F800000;// Store poly in scratchpad fast RAM in CPU.
	int num_vex_per_face = mesh->num_vex_per_face;

	setTAG(mesh, poly);
	/*
	setTAG(mesh, poly):
	//// Use macro function to set tag and code in POLY same approach as setting standard POLY_GT4. ////
	use switch(mesh->polygon_type) to set one of these{
		setPolyF3(poly);
		...
		setTile16(poly);
	}
	*/

	SetShadeTex(poly, (mesh->FLAG_TEXSHADE != 1) );// 1=shade off, 0=shade on
	SetSemiTrans(poly, (mesh->FLAG_SEMITRANSPARENT == 1) );
	
	//set uv0-3
	if(mesh->FLAG_TEXTURED){
		setUV(poly, mesh);
		/*
		Custom function, it does not care about 4th vertex if a primitive is using just 3 vertecies (like FT3). This means that 4th vertex data will copy garbage to POLY_X that will not be copied back from POLY_X to GPU primitive due to [code] value (like FT3). 
		*/
	}
	//set RGB0-3
	setColor(poly, mesh);// custom function
	
	if(num_vex_per_face == 4){
		// vex 4
		process_polygon4(mesh, poly);
	}else if(num_vex_per_face == 3){
		// vex 3
		process_polygon3(mesh, poly);
	}else  if(num_vex_per_face == 2){
		// vex 2
		process_polygon2(mesh, poly);
	}else{
		// vex 1
		process_polygon1(mesh, poly);
	}
}

// Other implementations are similar to process_polygon4(...).
void process_polygon4(Mesh *mesh, POLY_X *poly){// POLY_X prevents to take care of POLY_F4, POLY_FT4, POLY_G4, POLY_GT4 structs. This is handheld by primitivemapping(..., ...); 
	long p, otz, flag, nclip, vex_i_offset;

	for(vex_i_offset = 0 ; vex_i_offset < mesh->num_vertex ; vex_i_offset +=4 ){
		if(mesh->flag_use_nclip){
			nclip = RotAverageNclip4(mesh->vex + vex_i_offset, mesh->vex + vex_i_offset + 1, mesh->vex + vex_i_offset + 2, mesh->vex + vex_i_offset + 3,
					(long *) &poly->x0, // to screen space
					(long *) &poly->x1,
					(long *) &poly->x2,
					(long *) &poly->x3, &p, &otz, &flag);
		}else{
			nclip = 1;
			otz = RotAverage4(mesh->vex + vex_i_offset, mesh->vex + vex_i_offset + 1, mesh->vex + vex_i_offset + 2, mesh->vex + vex_i_offset + 3,
							(long *) &poly->x0, // to screen space
							(long *) &poly->x1,
							(long *) &poly->x2,
							(long *) &poly->x3, &p, &flag);
		}
		if(nclip > 0){
			/*
			coverts POLY_X into POLY_GT4 data type, because poly was set using setPolyGT4(poly);
			copy [poly] necessary variables from scratchpad POLY_X 56bytes struct into continues POLY_GT4 into 48bytes RAM (this RAM address is pointed by [next_empty_packetbuffer_slot]).
			[next_empty_packetbuffer_slot] is usually pointing into range of u_char *packetbuffer0 or u_char *packetbuffer1 where u_char *packetbuffer0 = (u_char packetbuffer0[32 * 1024]);
			*/
			primitivemapping(system->next_empty_packetbuffer_slot, poly);
			AddPrim(&system->ot[system->OTLEN - otz], system->next_empty_packetbuffer_slot);
			system->next_empty_packetbuffer_slot += (((u_char*)poly)[3] + 1) << 2;//increment by sizeof(primitive)
		}
	}
}

void process_polygon3(Mesh *mesh, POLY_X *poly){// POLY_X prevents to take care of POLY_F3, POLY_FT3, POLY_G3, POLY_GT3 structs. This is handheld by primitivemapping(..., ...); 
	long p, otz, flag, nclip, vex_i_offset;

	for(vex_i_offset = 0 ; vex_i_offset < mesh->num_vertex ; vex_i_offset +=3 ){
		if(mesh->flag_use_nclip){
			nclip = RotAverageNclip3(mesh->vex + vex_i_offset, mesh->vex + vex_i_offset + 1, mesh->vex + vex_i_offset + 2,
					(long *) &poly->x0, // to screen space
					(long *) &poly->x1,
					(long *) &poly->x2,&p, &otz, &flag);
		}else{
			nclip = 1;
			otz = RotAverage3(mesh->vex + vex_i_offset, mesh->vex + vex_i_offset + 1, mesh->vex + vex_i_offset + 2,
							(long *) &poly->x0, // to screen space
							(long *) &poly->x1,
							(long *) &poly->x2,&p, &flag);
		}
		if(nclip > 0){
			primitivemapping(system->next_empty_packetbuffer_slot, poly);
			AddPrim(&system->ot[system->OTLEN - otz], system->next_empty_packetbuffer_slot);
			system->next_empty_packetbuffer_slot += (((u_char*)poly)[3] + 1) << 2;//increment by sizeof(primitive)
		}
	}
}

process_polygon2(mesh, poly){...}
process_polygon1(mesh, poly){...}