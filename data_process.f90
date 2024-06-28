program process_airport_data
    implicit none
    integer, parameter :: sp = selected_real_kind(6, 37)  ! Single precision
    character(len=5), parameter :: start_airport = 'KDKX'
    real(sp), parameter :: R = 6371.0_sp  ! Radius of Earth in kilometers
    real(sp) :: start_lat, start_lon
    integer :: i, num_airports, status, progress
    character(len=100) :: line
    character(len=5) :: tag
    real(sp) :: lat, lon, distance
    real(sp), allocatable :: latitudes(:), longitudes(:)
    character(len=5), allocatable :: tags(:)
    real(sp), external :: haversine_distance

    ! Find the starting airport's coordinates
    open(unit=10, file='airport_coordinates.txt', status='old', action='read')
    num_airports = 0

    do
        read(10,'(A)', iostat=status) line
        if (status /= 0) exit
        num_airports = num_airports + 1
    end do
    rewind(10)

    allocate(latitudes(num_airports), longitudes(num_airports), tags(num_airports))
    
    i = 0
    do
        read(10,'(A)', iostat=status) line
        if (status /= 0) exit
        i = i + 1
        read(line, '(A5, 2F20.15)') tag, lon, lat
        tags(i) = tag
        longitudes(i) = lon
        latitudes(i) = lat
        if (tag == start_airport) then
            start_lat = lat
            start_lon = lon
        end if
    end do
    close(10)

    ! Open output file
    open(unit=20, file='output.txt', status='new', action='write')
    
    ! Write header
    write(20, '(A)') 'Airport Tag   Distance (km)'

    ! Calculate distances and write to file
    progress = 0
    do i = 1, num_airports
        if (tags(i) /= start_airport) then
            distance = haversine_distance(start_lat, start_lon, latitudes(i), longitudes(i))
            write(20, '(A5, 1X, F10.3)') trim(tags(i)), distance
        end if

        ! Update progress every 1%
        if (i * 100 / num_airports > progress) then
            progress = i * 100 / num_airports
            print *, 'Progress: ', progress, '%'
        end if
    end do

    close(20)
    print *, 'Processing complete.'

contains
    real(sp) function haversine_distance(lat1, lon1, lat2, lon2)
        real(sp), intent(in) :: lat1, lon1, lat2, lon2
        real(sp) :: dlat, dlon, a, c

        dlat = (lat2 - lat1) * (3.14159265358979323846_sp / 180.0_sp)
        dlon = (lon2 - lon1) * (3.14159265358979323846_sp / 180.0_sp)

        a = sin(dlat / 2.0_sp)**2 + cos(lat1 * (3.14159265358979323846_sp / 180.0_sp)) * &
            cos(lat2 * (3.14159265358979323846_sp / 180.0_sp)) * sin(dlon / 2.0_sp)**2
        c = 2.0_sp * atan2(sqrt(a), sqrt(1.0_sp - a))

        haversine_distance = R * c
    end function haversine_distance
end program process_airport_data
