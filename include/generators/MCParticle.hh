#ifndef MC_PARTICLE_HH
#define MC_PARTICLE_HH

#include "G4LorentzVector.hh"
#include "G4Step.hh"

#include "TLorentzVector.h"

#include <Eigen/Dense>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

struct TrajectoryHit {
    Eigen::Vector3d start_ = Eigen::Vector3d(0., 0., 0.);
    Eigen::Vector3d end_ = Eigen::Vector3d(0., 0., 0.);
    double      energy_deposit_ = 0;
    double      start_time_ = 0;
    double      end_time_ = 0;
    int         track_id_ = -1;
    int         pdg_code_ = -1;
    double      length_ = 0;
    std::string process_ = "unknown";

    Eigen::Vector3d StartPoint()  const { return start_; }
    Eigen::Vector3d EndPoint()    const { return end_;   }
    Eigen::Vector3d MidPoint()    const { return { (start_.x() + end_.x())/2.0,
                                           (start_.y() + end_.y())/2.0,
                                           (start_.z() + end_.z())/2.0 }; }
    double  StartTime()   const { return start_time_;     }
    double  EndTime()     const { return end_time_;       }
    double  Time()        const { return (start_time_ + end_time_)/2.0; }
    double  Energy()      const { return energy_deposit_; }
    double  TrackID()     const { return track_id_;       }
    double  PDGCode()     const { return pdg_code_;       }
    double  Length()      const { return length_;         }
    std::string Process() const { return process_; }
};

class MCParticle {
public:
    MCParticle();
    ~MCParticle();

    void AddTrajectoryHit(TrajectoryHit const &);
    void AddTrajectoryHit(G4Step const *);

    void AddDaughter(int const);

    inline std::vector<TrajectoryHit> Hits() const { return hits_; }

    inline int          NumberDaughters() const { return number_daughters_; }
    inline std::vector<int> Daughters() const { return daughter_track_ids_; }

    inline int         TrackID()        const { return track_id_;        }
    inline int         ParentTrackID()  const { return parent_track_id_; }
    inline int         PDGCode()        const { return pdg_code_;        }
    inline double      Mass()           const { return mass_;            }
    inline double      Charge()         const { return charge_;          }
    inline double      GlobalTime()     const { return global_time_;     }
    inline std::string Process()        const { return process_;         }
    inline int         TotalOccupancy() const { return total_occupancy_; }

    inline double EnergyDeposited() const { return energy_deposited_; }

    inline TLorentzVector InitialPosition() const { return initial_position_; }
    inline TLorentzVector InitialMomentum() const { return initial_momentum_; }

    inline void SetTrackID(int const trackID)               { track_id_ = trackID;               }
    inline void SetParentTrackID(int const parentTrackID)   { parent_track_id_ = parentTrackID;  }
    inline void SetPDGCode(int const pdgCode)               { pdg_code_ = pdgCode;               }
    inline void SetMass(double const mass)                  { mass_ = mass;                      }
    inline void SetCharge(double const charge)              { charge_ = charge;                  }
    inline void SetGlobalTime(double const globalTime)      { global_time_ = globalTime;         }
    inline void SetProcess(std::string const process)       { process_ = process;                }
    inline void SetTotalOccupancy(int const totalOccupancy) { total_occupancy_ = totalOccupancy; }

    inline void SetInitialPosition(TLorentzVector const initialPosition) { initial_position_ = initialPosition; }
    inline void SetInitialMomentum(TLorentzVector const initialMomentum) { initial_momentum_ = initialMomentum; }

private:
    int         track_id_;
    int         parent_track_id_;
    int         pdg_code_;
    double      mass_;
    double      charge_;
    double      global_time_;
    std::string process_;
    int         total_occupancy_;

    double      energy_deposited_ = 0.;

    TLorentzVector initial_position_;
    TLorentzVector initial_momentum_;

    std::vector<TrajectoryHit> hits_;

    int              number_daughters_ = 0;
    std::vector<int> daughter_track_ids_;
};

#endif // MC_PARTICLE_HH