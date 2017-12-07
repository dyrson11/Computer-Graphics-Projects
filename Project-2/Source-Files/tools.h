void loadSphere(char *archivo_dir)
{
	int linea;
	int indice = 0;
	ifstream archivo;
	float x, y, z;
    vec3 t, sum, tmp;
	archivo.open(archivo_dir);
	if (archivo.fail()) { cout << "el archivo no se abrio correctamente" << endl; exit(1); }

	int puntos = 0;
	archivo >> puntos;
	sphere_pos_vec.reserve(puntos * 9);
	for (size_t i = 0; i < puntos; i++)
	{
		sum = {0,0,0};
		archivo >> linea;
		for (int j = 0; j <linea; j++)
		{
			archivo >> x;
			sphere_pos_vec.push_back(x);
			sphere_col_vec.push_back(1.0);

			archivo >> y;
			sphere_pos_vec.push_back(y);
			sphere_col_vec.push_back(0.84);

			archivo >> z;
			sphere_pos_vec.push_back(z);
			sphere_col_vec.push_back(0.0);
            tmp = vec3( x, y, z);
            sum += tmp;
            sphereSmoothNorm.push_back(x);
            sphereSmoothNorm.push_back(y);
            sphereSmoothNorm.push_back(z);
		}
        vec3 point = normalize(sum);

        sphereFlatNorm.push_back(point.x); sphereFlatNorm.push_back(point.y); sphereFlatNorm.push_back(point.z);
        sphereFlatNorm.push_back(point.x); sphereFlatNorm.push_back(point.y); sphereFlatNorm.push_back(point.z);
        sphereFlatNorm.push_back(point.x); sphereFlatNorm.push_back(point.y); sphereFlatNorm.push_back(point.z);
	}
	archivo.close();
}


void floor()
{
	floor_colors[0] = vertex_colors[3]; floor_points[0] = vec3(5, 0, 8);
	floor_colors[1] = vertex_colors[3]; floor_points[1] = vec3(5, 0, -4);
	floor_colors[2] = vertex_colors[3]; floor_points[2] = vec3(-5, 0, -4);

	floor_colors[3] = vertex_colors[3]; floor_points[3] = vec3(5, 0, 8);
	floor_colors[4] = vertex_colors[3]; floor_points[4] = vec3(-5, 0, -4);
	floor_colors[5] = vertex_colors[3]; floor_points[5] = vec3(-5, 0, 8);
}

void lines()
{
	x_colors[0] = vertex_colors[1]; x_points[0] = vec3(0, 0, 0);
	x_colors[1] = vertex_colors[1]; x_points[1] = vec3(0, 0.05, 0);
	x_colors[2] = vertex_colors[1]; x_points[2] = vec3(100, 0, 0);

	y_colors[0] = vertex_colors[5]; y_points[0] = vec3(0, 0, 0);
	y_colors[1] = vertex_colors[5]; y_points[1] = vec3(0.05, 0.0, 0);
	y_colors[2] = vertex_colors[5]; y_points[2] = vec3(0, 100, 0);

	z_colors[0] = vertex_colors[4]; z_points[0] = vec3(0, 0, 0);
	z_colors[1] = vertex_colors[4]; z_points[1] = vec3(0, 0.05, 0);
	z_colors[2] = vertex_colors[4]; z_points[2] = vec3(0, 0, 100);
}
