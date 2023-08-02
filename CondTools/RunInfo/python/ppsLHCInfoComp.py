from datetime import datetime
import time

def to_unix_timestamp(date_string):
    # try:
    date_format = "%d/%m/%Y %H:%M:%S"
    datetime_obj = datetime.strptime(date_string, date_format)
    unix_timestamp = int(time.mktime(datetime_obj.timetuple()))
    return unix_timestamp
    # except ValueError:
    #     throw ValueError("Invalid date format. Please provide date string in the format 'DD/MM/YYYY HH:MI:SS'.")

def to_date_string(unix_timestamp):
    # try:
    datetime_obj = datetime.fromtimestamp(unix_timestamp)
    date_string = datetime_obj.strftime("%d/%m/%Y %H:%M:%S")
    return date_string
    # except ValueError:
    #     print("Invalid timestamp. Please provide a valid Unix timestamp.")


with open('8614_pps.txt') as f_pps:
    with open('8614_content_csv_ext.out') as f_lhc:
        print("fill", "run", "LS", "beta*", "xangle", "dip date", "record", "LS", "beta", "xangle", "delivLumi", "IOV", "IOV date", "beta* error", "xangle error", "", sep = ";")

        lhc_beta = None
        lhc_xangle = None

        line_lhc = f_lhc.readline()
        #skip the headers
        if "IOV" in line_lhc or line_lhc == "\n":
            # print("skipping the header") #TODO remove
            line_lhc = f_lhc.readline()

        split_lhc = line_lhc.split(';')

        for line_pps in f_pps:
            line_pps = line_pps[1:-2]
            split_pps = line_pps.split(', ')

            for i, val in enumerate(split_pps):
                print(val, end = ";" if i <= 5 or i == len(split_pps)-1 else ", ")
            


            # print("  pps:", (split_pps[2]), "lhc:", (split_lhc[2]))
            if(len(split_pps) > 5 and len(split_lhc) > 5):
                # if len(split_pps[5][1:-1]) != len('17/04/2023 07:30:42'):
                #     print(split_pps[5][1:-1], len(split_pps[5][1:-1]), len('17/04/2023 07:30:42'))
                #     print(split_pps[5][1:-1])
                #     print('17/04/2023 07:30:42')
                #     exit(0)
                pps_timestamp = to_unix_timestamp(split_pps[5][1:-1]) + 2*60*60 # - coversion of time zones
                # print(pps_timestamp, "-", int(split_lhc[2]), "=", pps_timestamp -int(split_lhc[2]), end = " |; ")
                if(abs(pps_timestamp-int(split_lhc[2])) < 10*60 and split_pps[2] == split_lhc[3]):
                    # print(line_lhc, end="")
                    print(split_lhc[1], split_lhc[3], split_lhc[5], split_lhc[4], split_lhc[6],
                        split_lhc[0], to_date_string(int(split_lhc[2])), sep=";", end=";")
                    lhc_beta = float(split_lhc[5])
                    lhc_xangle = float(split_lhc[4])

                    line_lhc = f_lhc.readline()
                    split_lhc = line_lhc.split(';')
                else:
                    print(";;;;;;;", end = "")
                
            #                 lhc_beta = lhc_beta
            # lhc_xangle = lhc_xangle
                pps_beta = float(split_pps[3])
                pps_xangle = float(split_pps[4])
                # print(pps_beta, lhc_beta, pps_xangle, lhc_xangle, sep=" - ", end = " | ")
                print(abs(pps_beta-lhc_beta) if lhc_beta is not None else 0,
                    abs(pps_xangle-lhc_xangle) if lhc_xangle is not None else 0, "", sep = ";")

                