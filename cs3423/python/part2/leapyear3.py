for year in [y for y in range(1900, 2000) if (y % 4 == 0 and y % 100 != 0) or (y % 400 == 0)]:
    print(year)
