#include "RecoCTPPS/TotemRPLocal/interface/CalibrationData.h"

std::ostream& operator<<(std::ostream &strm, const CalibrationData::CalibrationKey &key){
  return strm << key.db << " " << key.sampic << " " << key.channel << " " << key.cell;
}

std::ostream& operator<<(std::ostream &strm, const CalibrationData &data){
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
    CalibrationData::CalibrationKey k = kv.first;
    k.cell = -1;
    strm << "] " << data.timeOffsets_.at(k) << "\n";
  }
  return strm;
}

CalibrationData::CalibrationData()
: formula_(""), parameters_(std::map<CalibrationKey, std::vector<double>>()), timeOffsets_(std::map<CalibrationKey, double>()){}

void CalibrationData::openFile(const std::string &file_name){
  pt::ptree node;
  pt::read_json(file_name, node); //WARNING: void function that throws json_parser_error
  formula_ = node.get<std::string>("formula");

  for(pt::ptree::value_type &par : node.get_child("parameters")){
    CalibrationKey key = CalibrationKey();

    key.db = (int) strtol(par.first.data(), nullptr, 10);

    for(pt::ptree::value_type &board : par.second) {

      key.sampic = board.second.get<int>("sampic"); // -1
      key.channel = board.second.get<int>("channel");
      double timeOffset = board.second.get<double>("time_offset");

      key.cell = -1;
      timeOffsets_.insert(std::pair<CalibrationKey, double >(key, timeOffset));

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

void CalibrationData::print(){
  std::cout << *this;
}

std::vector<double> CalibrationData::getParameters(int db, int sampic, int channel, int cell) const{
  CalibrationKey key = CalibrationKey(db, sampic, channel, cell);
  auto out = parameters_.find(key);
  if(out == parameters_.end())
    return {};
  else
    return out->second;
}
double CalibrationData::getTimeOffset(int db, int sampic, int channel) const{
  CalibrationKey key = CalibrationKey(db, sampic, channel);
  auto out = timeOffsets_.find(key);
  if(out == timeOffsets_.end())
    return 0.0;
  else
    return out->second;
}

std::string CalibrationData::getFormula() const{
  return formula_;
}
