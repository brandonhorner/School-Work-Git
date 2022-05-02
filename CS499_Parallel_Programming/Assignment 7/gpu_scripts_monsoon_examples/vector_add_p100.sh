#!/bin/bash
#SBATCH --job-name=cuda_vector_add  # the name of your job
#SBATCH --output=/scratch/mg2745/monsoon_GPU_arch_test/cuda_vector_add_p100.out # this is the file for stdout
#SBATCH --error=/scratch/mg2745/monsoon_GPU_arch_test/cuda_vector_add_p100.err # this is the file for stdout
#SBATCH --time=00:02:00		# 2 min, shorter time, quicker start
#SBATCH --mem=1024         #1 GiB memory requested
#SBATCH --gres=gpu:tesla:1 #resource requirement
#SBATCH --qos=gpu_class
#SBATCH --constraint=p100



# load cuda
module load cuda/8.0

# srun hostname
# srun nvidia-smi

# run your application, precede the application command with srun
srun nvprof ~/CS499/Monsoon_GPU_arch_test/vector_add/vector_add_p100
# srun ~/CS499/Monsoon_GPU_arch_test/vector_add/vector_add_p100



