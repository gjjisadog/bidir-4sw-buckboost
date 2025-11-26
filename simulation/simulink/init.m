function init()
% init  Add simulation helpers to path and place default params in base workspace.
%
% Usage:
%   >> cd(simulation/simulink); init;
%   (optionally run build_mex first if buckboost_sfunc is absent)

    root = fileparts(mfilename('fullpath'));
    addpath(root);

    params = struct();
    params.pwm_hz      = 20000;
    params.Ts          = 1 / params.pwm_hz;
    params.vo_ref_v    = 400;
    params.vin_ov_v    = 32;
    params.vin_uv_v    = 8;
    params.il_oc_a     = 10;
    params.duty_max    = 0.95;

    assignin('base', 'bb_params', params);
    assignin('base', 'bb_sample_time', params.Ts);

    if exist('buckboost_sfunc', 'file') ~= 3
        warning('buckboost_sfunc not found. Run build_mex to compile the S-function.');
    end
end

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
