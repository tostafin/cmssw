import FWCore.ParameterSet.Config as cms

ppsAlignmentConfigESSource = cms.ESSource("PPSAlignmentConfigurationESSource",
    sector_45 = cms.PSet(
        rp_N = cms.PSet(
            sh_x = cms.double(0.),

            x_min_fit_mode = cms.double(-5.),
            x_max_fit_mode = cms.double(8.),
            y_max_fit_mode = cms.double(8.),

            x_slice_min = cms.double(9.),
            x_slice_max = cms.double(16.),
            y_cen_add = cms.double(0.),
            y_width_mult = cms.double(1.0),
        ),
        rp_F = cms.PSet(
            sh_x = cms.double(0.),

            x_min_fit_mode = cms.double(-5.),
            x_max_fit_mode = cms.double(8.),
            y_max_fit_mode = cms.double(8.),

            x_slice_min = cms.double(3.),
            x_slice_max = cms.double(10.),
            y_cen_add = cms.double(0.),
            y_width_mult = cms.double(1.0),
        ),

        cut_h_a = cms.double(-1),
        cut_h_c = cms.double(1),

        cut_v_a = cms.double(-1.04),
        cut_v_c = cms.double(0)
    ),

    sector_56 = cms.PSet(
        rp_N = cms.PSet(
            sh_x = cms.double(0.),

            x_min_fit_mode = cms.double(-5.),
            x_max_fit_mode = cms.double(5.),
            y_max_fit_mode = cms.double(5.),

            x_slice_min = cms.double(3.),
            x_slice_max = cms.double(16.),
            y_cen_add = cms.double(0.),
            y_width_mult = cms.double(1.0),
        ),
        rp_F = cms.PSet(
            sh_x = cms.double(0.),

            x_min_fit_mode = cms.double(-5.),
            x_max_fit_mode = cms.double(5.),
            y_max_fit_mode = cms.double(5.),

            x_slice_min = cms.double(3.),
            x_slice_max = cms.double(16.),
            y_cen_add = cms.double(0.),
            y_width_mult = cms.double(1.0),
        ),

        cut_h_a = cms.double(-1),
        cut_h_c = cms.double(0),

        cut_v_a = cms.double(-1.05),
        cut_v_c = cms.double(1.4)
    ),

    binning = cms.PSet(
        pixel_x_offset = cms.double(0.)
    ),

    matching = cms.PSet(
        rp_L_N = cms.PSet(
            sh_min = cms.double(-2.),
            sh_max = cms.double(2.)
        ),
        rp_L_F = cms.PSet(
            sh_min = cms.double(-2.),
            sh_max = cms.double(2.)
        ),
        rp_R_N = cms.PSet(
            sh_min = cms.double(-2.),
            sh_max = cms.double(2.)
        ),
        rp_R_F = cms.PSet(
            sh_min = cms.double(-2.),
            sh_max = cms.double(2.)
        )
    ),

    x_alignment_meth_o = cms.PSet(
        rp_L_F = cms.PSet(
            x_min = cms.double(3.8),
            x_max = cms.double(10.),
        ),
        rp_L_N = cms.PSet(
            x_min = cms.double(9.),
            x_max = cms.double(15.),
        ),
        rp_R_N = cms.PSet(
            x_min = cms.double(5.5),
            x_max = cms.double(10.),
        ),
        rp_R_F = cms.PSet(
            x_min = cms.double(5.),
            x_max = cms.double(11.),
        )
    ),

    x_alignment_relative = cms.PSet(
        rp_L_N = cms.PSet(
            x_min = cms.double(4.),
            x_max = cms.double(12.)
        ),
        rp_R_N = cms.PSet(
            x_min = cms.double(4.),
            x_max = cms.double(12.)
        ),
    ),

    y_alignment = cms.PSet(
        rp_L_F = cms.PSet(
            x_min = cms.double(3.),
            x_max = cms.double(9.)
        ),
        rp_L_N = cms.PSet(
            x_min = cms.double(3.5),
            x_max = cms.double(6.5)
        ),
        rp_R_N = cms.PSet(
            x_min = cms.double(2.6),
            x_max = cms.double(6.)
        ),
        rp_R_F = cms.PSet(
            x_min = cms.double(2.6),
            x_max = cms.double(6.)
        )
    )
)
