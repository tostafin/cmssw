import sys
from datetime import datetime
import time
# print 'Number of arguments:', len(sys.argv), 

def to_date_string(unix_timestamp):
    datetime_obj = datetime.fromtimestamp(unix_timestamp)
    date_string = datetime_obj.strftime("%d/%m/%Y %H:%M:%S")
    return date_string

timestamp = int(sys.argv[1])
print(timestamp)
unix_timestamp = timestamp >> 32
print(unix_timestamp)
print(to_date_string(unix_timestamp))
