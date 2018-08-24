#include "RecoCTPPS/TotemRPLocal/interface/TotemTimingParser.h"

TotemTimingParser::TotemTimingParser()
: formula_(""), parameters_(std::map<CalibrationKey, std::vector<double>>()),
timeInfo_(std::map<CalibrationKey, std::pair<double, double >>()){}

void TotemTimingParser::parseFile(const std::string &file_name){
  pt::ptree node;
  pt::read_json(file_name, node);
  formula_ = node.get<std::string>("formula");

  for(pt::ptree::value_type &par : node.get_child("parameters")){
    CalibrationKey key = CalibrationKey();
    key.db = (int) strtol(par.first.data(), nullptr, 10);

    for(pt::ptree::value_type &board : par.second) {
      key.sampic = board.second.get<int>("sampic");
      key.channel = board.second.get<int>("channel");
      double timeOffset = board.second.get<double>("time_offset");
      double timePrecision = board.second.get<double>("time_precision");
      std::pair<double, double > timeInfo = std::pair<double, double >(timeOffset, timePrecision);
      key.cell = -1;
      timeInfo_.insert(std::pair<CalibrationKey, std::pair<double, double > >(key, timeInfo));

      int cell_ct = 0;
      for (pt::ptree::value_type &cell : board.second.get_child("cells")) {
        std::vector<double> values;
        key.cell = cell_ct;

        for (pt::ptree::value_type &param : cell.second) {
          values.push_back((double) strtod(param.second.data().c_str(), NULL));
        }
        parameters_.insert(std::pair<CalibrationKey, std::vector<double> >(key, values));
        cell_ct++;
      }
    }
  }
}

std::vector<double> TotemTimingParser::getParameters(int db, int sampic, int channel, int cell) const{
  CalibrationKey key = CalibrationKey(db, sampic, channel, cell);
  auto out = parameters_.find(key);
  if(out == parameters_.end())
    return {};
  else
    return out->second;
}
double TotemTimingParser::getTimeOffset(int db, int sampic, int channel) const{
  CalibrationKey key = CalibrationKey(db, sampic, channel);
  auto out = timeInfo_.find(key);
  if(out == timeInfo_.end())
    return 0.0;
  else
    return out->second.first;
}

double TotemTimingParser::getTimePrecision(int db, int sampic, int channel) const{
  CalibrationKey key = CalibrationKey(db, sampic, channel);
  auto out = timeInfo_.find(key);
  if(out == timeInfo_.end())
    return 0.0;
  else
    return out->second.second;
}

std::string TotemTimingParser::getFormula() const{
  return formula_;
}

std::ostream& operator<<(std::ostream &strm, const TotemTimingParser::CalibrationKey &key){
  return strm << key.db << " " << key.sampic << " " << key.channel << " " << key.cell;
}

std::ostream& operator<<(std::ostream &strm, const TotemTimingParser &data){
  strm << "FORMULA: "<< data.formula_ << "\nDB SAMPIC CHANNEL CELL PARAMETERS TIME_OFFSET\n";
  for (auto& kv : data.parameters_) {
    strm << kv.first <<" [";
    bool first = true;
    for (auto& a : kv.second){
      if(first){
        strm << a;
        first = false;
      }
      else
      strm << ", " << a ;
    }
    TotemTimingParser::CalibrationKey k = kv.first;
    k.cell = -1;
    strm << "] " << data.timeInfo_.at(k).first << " " <<  data.timeInfo_.at(k).second << "\n";
  }
  return strm;
}
