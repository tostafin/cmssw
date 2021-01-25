#include "CondFormats/PPSObjects/interface/PPSTopology.h"
//#include <iostream>

// Constructors

PPSTopology::PPSTopology()
  :  pitch_simY_(0.),
pitch_simX_(0.),
thickness_(0.),
no_of_pixels_simX_(0.),
no_of_pixels_simY_(0.),
no_of_pixels_(0.),
simX_width_(0.),
simY_width_(0.),
dead_edge_width_(0.),
active_edge_sigma_(0.),
phys_active_edge_dist_(0.) 
{
  active_edge_x_ = simX_width_ * 0.5 - phys_active_edge_dist_;
  active_edge_y_ = simY_width_ * 0.5 - phys_active_edge_dist_;
}

// Destructor
PPSTopology::~PPSTopology() {}


unsigned short PPSTopology::pixelIndex(PixelInfo pI){
    return no_of_pixels_simX_ * pI.pixelColNo() + pI.pixelRowNo();
  }

bool PPSTopology::isPixelHit(float xLocalCoordinate, float yLocalCoordinate, bool is3x2 = true) {
  // check hit fiducial boundaries
  double xModuleSize = 2 * ((no_of_pixels_simX_ / 2. + 1) * pitch_simX_ + dead_edge_width_);
  if (xLocalCoordinate < -xModuleSize / 2. || xLocalCoordinate > xModuleSize / 2.)
    return false;
  
  double yModuleSize = (no_of_pixels_simY_ + 4.) * pitch_simY_ + 2. * dead_edge_width_;
  double y2x2top = no_of_pixels_simY_ / 6. * pitch_simY_ + dead_edge_width_;
  if (is3x2 && (yLocalCoordinate < -yModuleSize / 2. || yLocalCoordinate > yModuleSize / 2.))
    return false;
  
  if (!is3x2 && (yLocalCoordinate < -yModuleSize / 2. || yLocalCoordinate > y2x2top))
    return false;
  
  return true;
}

PPSTopology::PixelInfo PPSTopology::getPixelsInvolved(
						      double x, double y, double sigma, double& hit_pos_x, double& hit_pos_y) const {
  //hit position wrt the bottom left corner of the sensor (-8.3, -12.2) in sensor view, rocs behind
  hit_pos_x = x + simX_width_ / 2.;
  hit_pos_y = y + simY_width_ / 2.;
  if (!(hit_pos_x * hit_pos_y > 0))
    throw cms::Exception("PPSTopology") << "pixel out of reference frame";

  double hit_factor = activeEdgeFactor(x, y);

  unsigned int interested_row = row(x);
  unsigned int interested_col = col(y);
  double low_pixel_range_x, high_pixel_range_x, low_pixel_range_y, high_pixel_range_y;
  pixelRange(
	     interested_row, interested_col, low_pixel_range_x, high_pixel_range_x, low_pixel_range_y, high_pixel_range_y);

  return PPSTopology::PixelInfo(low_pixel_range_x,
				high_pixel_range_x,
				low_pixel_range_y,
				high_pixel_range_y,
				hit_factor,
				interested_row,
				interested_col);
}

void PPSTopology::pixelRange(unsigned int arow,
			     unsigned int acol,
			     double& lower_x,
			     double& higher_x,
			     double& lower_y,
			     double& higher_y) const {
  // x and y in the system  of Geant4 SIMULATION
  arow = (2 * ROCSizeInX - 1) - arow;
  if (arow > (2 * ROCSizeInX - 1) || acol > (3 * ROCSizeInY - 1))
    throw cms::Exception("PPSTopology") << "pixel rows or columns exceeding limits";

  // rows (x segmentation)
  if (arow == 0) {
    lower_x = dead_edge_width_ - phys_active_edge_dist_;  // 50 um
    higher_x = dead_edge_width_ + pitch_simX_;            // 300 um
  } else if (arow <= (ROCSizeInX - 2)) {
    lower_x = dead_edge_width_ + arow * pitch_simX_;
    higher_x = dead_edge_width_ + (arow + 1) * pitch_simX_;
  } else if (arow == (ROCSizeInX - 1)) {
    lower_x = dead_edge_width_ + arow * pitch_simX_;
    higher_x = dead_edge_width_ + (arow + 2) * pitch_simX_;
  } else if (arow == ROCSizeInX) {
    lower_x = dead_edge_width_ + (arow + 1) * pitch_simX_;
    higher_x = dead_edge_width_ + (arow + 3) * pitch_simX_;
  } else if (arow <= (2 * ROCSizeInX - 2)) {
    lower_x = dead_edge_width_ + (arow + 2) * pitch_simX_;
    higher_x = dead_edge_width_ + (arow + 3) * pitch_simX_;
  } else if (arow == (2 * ROCSizeInX - 1)) {
    lower_x = dead_edge_width_ + (arow + 2) * pitch_simX_;
    higher_x = dead_edge_width_ + (arow + 3) * pitch_simX_ + phys_active_edge_dist_;
  }
  // columns (y segmentation)
  if (acol == 0) {
    lower_y = dead_edge_width_ - phys_active_edge_dist_;  // 50 um
    higher_y = dead_edge_width_ + pitch_simY_;            // 350 um
  } else if (acol <= (ROCSizeInY - 2)) {
    lower_y = dead_edge_width_ + acol * pitch_simY_;
    higher_y = dead_edge_width_ + (acol + 1) * pitch_simY_;
  } else if (acol == (ROCSizeInY - 1)) {
    lower_y = dead_edge_width_ + acol * pitch_simY_;
    higher_y = dead_edge_width_ + (acol + 2) * pitch_simY_;
  } else if (acol == ROCSizeInY) {
    lower_y = dead_edge_width_ + (acol + 1) * pitch_simY_;
    higher_y = dead_edge_width_ + (acol + 3) * pitch_simY_;
  } else if (acol <= (2 * ROCSizeInY - 2)) {
    lower_y = dead_edge_width_ + (acol + 2) * pitch_simY_;
    higher_y = dead_edge_width_ + (acol + 3) * pitch_simY_;
  } else if (acol == (2 * ROCSizeInY - 1)) {
    lower_y = dead_edge_width_ + (acol + 2) * pitch_simY_;
    higher_y = dead_edge_width_ + (acol + 4) * pitch_simY_;
  } else if (acol == (2 * ROCSizeInY)) {
    lower_y = dead_edge_width_ + (acol + 3) * pitch_simY_;
    higher_y = dead_edge_width_ + (acol + 5) * pitch_simY_;
  } else if (acol <= (3 * ROCSizeInY - 2)) {
    lower_y = dead_edge_width_ + (acol + 4) * pitch_simY_;
    higher_y = dead_edge_width_ + (acol + 5) * pitch_simY_;
  } else if (acol == (3 * ROCSizeInY - 1)) {
    lower_y = dead_edge_width_ + (acol + 4) * pitch_simY_;
    higher_y = dead_edge_width_ + (acol + 5) * pitch_simY_ + phys_active_edge_dist_;
  }

  lower_x = lower_x - simX_width_ / 2.;
  lower_y = lower_y - simY_width_ / 2.;
  higher_x = higher_x - simX_width_ / 2.;
  higher_y = higher_y - simY_width_ / 2.;
}

double PPSTopology::activeEdgeFactor(double x, double y) const {
  const double inv_sigma = 1. / active_edge_sigma_;  // precaching
  const double topEdgeFactor = std::erf(-distanceFromTopActiveEdge(x, y) * inv_sigma) * 0.5 + 0.5;
  const double bottomEdgeFactor = std::erf(-distanceFromBottomActiveEdge(x, y) * inv_sigma) * 0.5 + 0.5;
  const double rightEdgeFactor = std::erf(-distanceFromRightActiveEdge(x, y) * inv_sigma) * 0.5 + 0.5;
  const double leftEdgeFactor = std::erf(-distanceFromLeftActiveEdge(x, y) * inv_sigma) * 0.5 + 0.5;

  const double aEF = topEdgeFactor * bottomEdgeFactor * rightEdgeFactor * leftEdgeFactor;

  if (aEF > 1.)
    throw cms::Exception("PPSTopology") << " pixel active edge factor > 1";

  return aEF;
}

double PPSTopology::distanceFromTopActiveEdge(double x, double y) const { return (y - active_edge_y_); }
double PPSTopology::distanceFromBottomActiveEdge(double x, double y) const { return (-y - active_edge_y_); }
double PPSTopology::distanceFromRightActiveEdge(double x, double y) const { return (x - active_edge_x_); }
double PPSTopology::distanceFromLeftActiveEdge(double x, double y) const { return (-x - active_edge_x_); }

unsigned int PPSTopology::row(double x) const {
  // x in the G4 simulation system
  x = x + simX_width_ / 2.;

  // now x in the system centered in the bottom left corner of the sensor (sensor view, rocs behind)
  if (x < 0. || x > simX_width_)
    throw cms::Exception("PPSTopology") << " pixel out of reference frame";

  // rows (x segmentation)
  unsigned int arow;
  if (x <= (dead_edge_width_ + pitch_simX_))
    arow = 0;
  else if (x <= (dead_edge_width_ + (ROCSizeInX - 1) * pitch_simX_))
    arow = int((x - dead_edge_width_ - pitch_simX_) / pitch_simX_) + 1;
  else if (x <= (dead_edge_width_ + (ROCSizeInX + 1) * pitch_simX_))
    arow = (ROCSizeInX - 1);
  else if (x <= (dead_edge_width_ + (ROCSizeInX + 3) * pitch_simX_))
    arow = ROCSizeInX;
  else if (x <= (dead_edge_width_ + (2 * ROCSizeInX + 2) * pitch_simX_))
    arow = int((x - dead_edge_width_ - pitch_simX_) / pitch_simX_) - 1;
  else
    arow = (2 * ROCSizeInX - 1);

  arow = (2 * ROCSizeInX - 1) - arow;
  if (arow > (2 * ROCSizeInX - 1))
    throw cms::Exception("PPSTopology") << " pixel row number exceeding limit";

  return arow;
}

unsigned int PPSTopology::col(double y) const {
  // y in the G4 simulation system
  unsigned int column;

  // columns (y segmentation)
  // now y in the system centered in the bottom left corner of the sensor (sensor view, rocs behind)
  y = y + simY_width_ / 2.;
  if (y < 0. || y > simY_width_)
    throw cms::Exception("PPSTopology") << "pixel out of reference frame";

  if (y <= (dead_edge_width_ + pitch_simY_))
    column = 0;
  else if (y <= (dead_edge_width_ + (ROCSizeInY - 1) * pitch_simY_))
    column = int((y - dead_edge_width_ - pitch_simY_) / pitch_simY_) + 1;
  else if (y <= (dead_edge_width_ + (ROCSizeInY + 1) * pitch_simY_))
    column = ROCSizeInY - 1;
  else if (y <= (dead_edge_width_ + (ROCSizeInY + 3) * pitch_simY_))
    column = ROCSizeInY;
  else if (y <= (dead_edge_width_ + (2 * ROCSizeInY + 1) * pitch_simY_))
    column = int((y - dead_edge_width_ - pitch_simY_) / pitch_simY_) - 1;
  else if (y <= (dead_edge_width_ + (2 * ROCSizeInY + 3) * pitch_simY_))
    column = 2 * ROCSizeInY - 1;
  else if (y <= (dead_edge_width_ + (2 * ROCSizeInY + 5) * pitch_simY_))
    column = 2 * ROCSizeInY;
  else if (y <= (dead_edge_width_ + (3 * ROCSizeInY + 3) * pitch_simY_))
    column = int((y - dead_edge_width_ - pitch_simY_) / pitch_simY_) - 3;
  else
    column = (3 * ROCSizeInY - 1);

  return column;
}

void PPSTopology::rowCol2Index(unsigned int arow, unsigned int acol, unsigned int& index) const {
  index = acol * no_of_pixels_simX_ + arow;
}

void PPSTopology::index2RowCol(unsigned int& arow, unsigned int& acol, unsigned int index) const {
  acol = index / no_of_pixels_simX_;
  arow = index % no_of_pixels_simX_;
}

// Getters

//double PPSTopology::getBeamMom45() const { return beam_momentum_45_; }
double PPSTopology::getPitchSimY() const { return pitch_simY_; }
double PPSTopology::getPitchSimX() const { return pitch_simX_; }
double PPSTopology::getThickness() const { return thickness_; }
unsigned short PPSTopology::getNoPixelsSimX() const { return no_of_pixels_simX_; }
unsigned short PPSTopology::getNoPixelsSimY() const { return no_of_pixels_simY_; }
unsigned short PPSTopology::getNoPixels() const { return no_of_pixels_; }
double PPSTopology::getSimXWidth() const { return simX_width_; }
double PPSTopology::getSimYWidth() const { return simY_width_; }
double PPSTopology::getDeadEdgeWidth() const { return dead_edge_width_; }
double PPSTopology::getActiveEdgeSigma() const { return active_edge_sigma_; }
double PPSTopology::getPhysActiveEdgeDist() const { return phys_active_edge_dist_; }

// Setters

//void PPSTopology::setBeamMom45(double mom) { beam_momentum_45_ = mom; }
void PPSTopology::setPitchSimY(double psy){pitch_simY_ = psy;}
void PPSTopology::setPitchSimX(double psx){pitch_simX_ = psx;}
void PPSTopology::setThickness(double tss){thickness_ = tss;}
void PPSTopology::setNoPixelsSimX(unsigned short npx){no_of_pixels_simX_ = npx;}
  void PPSTopology::setNoPixelsSimY(unsigned short npy){no_of_pixels_simY_ = npy;}
  void PPSTopology::setNoPixels(unsigned short np){no_of_pixels_ = np;}
  void PPSTopology::setSimXWidth(double sxw){simX_width_ = sxw;}
  void PPSTopology::setSimYWidth(double syw){simY_width_ = syw;}
  void PPSTopology::setDeadEdgeWidth(double dew){dead_edge_width_ = dew;}
  void PPSTopology::setActiveEdgeSigma(double aes){active_edge_sigma_ = aes;}
  void PPSTopology::setPhysActiveEdgeDist(double pae){phys_active_edge_dist_ = pae;}

  void PPSTopology::printInfo(std::stringstream& s) {
  s << "\n PPS Topology parameters : \n"
    /* << "\n   beam_momentum_45 = " << beam_momentum_45_ << " GeV"
  << "\n   beam_momentum_56 = " << beam_momentum_56_ << " GeV"
  << "\n   beta_star_x_45 = " << beta_star_x_45_ << " cm"
  << "\n   beta_star_y_45 = " << beta_star_y_45_ << " cm"
  << "\n   beta_star_x_56 = " << beta_star_x_56_ << " cm"
  << "\n   beta_star_y_56 = " << beta_star_y_56_ << " cm"
  << "\n   beam_divergence_x_45 = " << beam_divergence_x_45_ << " rad"
  << "\n   beam_divergence_y_45 = " << beam_divergence_y_45_ << " rad"
  << "\n   beam_divergence_x_56 = " << beam_divergence_x_56_ << " rad"
  << "\n   beam_divergence_y_56 = " << beam_divergence_y_56_ << " rad"
  << "\n   half_crossing_angle_x_45 = " << half_crossing_angle_x_45_ << " rad"
  << "\n   half_crossing_angle_y_45 = " << half_crossing_angle_y_45_ << " rad"
  << "\n   half_crossing_angle_x_56 = " << half_crossing_angle_x_56_ << " rad"
  << "\n   half_crossing_angle_y_56 = " << half_crossing_angle_y_56_ << " rad"
  << "\n   vtx_offset_x_45 = " << vtx_offset_x_45_ << " cm"
  << "\n   vtx_offset_y_45 = " << vtx_offset_y_45_ << " cm"
  << "\n   vtx_offset_z_45 = " << vtx_offset_z_45_ << " cm"
  << "\n   vtx_offset_t_45 = " << vtx_offset_t_45_ << " cm"
  << "\n   vtx_offset_x_56 = " << vtx_offset_x_56_ << " cm"
  << "\n   vtx_offset_y_56 = " << vtx_offset_y_56_ << " cm"
  << "\n   vtx_offset_z_56 = " << vtx_offset_z_56_ << " cm"
  << "\n   vtx_offset_t_56 = " << vtx_offset_t_56_ << " cm"
  << "\n   vtx_stddev_x = " << vtx_stddev_x_ << " cm"
  << "\n   vtx_stddev_y = " << vtx_stddev_y_ << " cm"
  << "\n   vtx_stddev_z = " << vtx_stddev_z_ << " cm"
  << "\n   vtx_stddev_t = " << vtx_stddev_t_ << " cm" */
<< std::endl;
  }


  std::ostream& operator<<(std::ostream& os, PPSTopology info) {
  std::stringstream ss;
  info.printInfo(ss);
  os << ss.str();
  return os;
  }

