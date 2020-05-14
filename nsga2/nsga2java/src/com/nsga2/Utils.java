package src.com.nsga2;

import src.com.nsga2.Population;
import src.com.nsga2.Problem;
import src.com.nsga2.Individual;
import java.util.Random;


class Utils {

    Problem problem;
    int numOfIndividuals;
    double mutationStrength;
    int numOfGenToMutate;
    int numOfTourParticips;

    Utils(UtilsBuilder builder) {
        this.problem = builder.problem;
        this.mutationStrength = builder.mutationStrength;
        this.numOfGenToMutate = builder.numOfGenToMutate;
        this.numOfTourParticips = builder.numOfTourParticips;
        this.numOfIndividuals = builder.numOfIndividuals;
    }

    private void crossover(Individual ind1, Individual ind2) {

    }

    /* this is the recommanded solution for avoiding so called "telescoping constructor pattern",
     * which is enssentially to avoid passing a bunch of arguments to the constructor.
     * But isn't it even more verbose ???
     * Java :( */
    public static class UtilsBuilder {
        private Problem problem;
        private int numOfIndividuals;
        private double mutationStrength;
        private int numOfGenToMutate;
        private int numOfTourParticips;

        public UtilsBuilder setProblem(Problem problem) {
            this.problem = problem;
            return this;
        }

        public UtilsBuilder setNumOfIndividuals(int numOfIndividuals) {
            this.numOfIndividuals = numOfIndividuals;
            return this;
        }

        public UtilsBuilder setMutationStrength(double mutationStrength) {
            this.mutationStrength = mutationStrength;
            return this;
        }

        public UtilsBuilder setNumOfGenToMutate(int numOfGenToMutate) {
            this.numOfGenToMutate = numOfGenToMutate;
            return this;
        }

        public UtilsBuilder setNumOfTourParticips(int numOfTourParticips) {
            this.numOfTourParticips = numOfTourParticips;
            return this;
        }
    }

}

