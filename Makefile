all: create_trimmed_list create_distances

create_trimmed_list: create_trimmed_list.cpp
	g++ -o create_trimmed_list create_trimmed_list.cpp

create_distances: create_distances.cpp
	g++ -o create_distances create_distances.cpp

clean:
	rm -f create_trimmed_list create_distances
