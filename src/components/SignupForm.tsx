import { useState } from 'react';
import { motion, AnimatePresence } from 'motion/react';
import { Check } from 'lucide-react';
import { SocialLogin } from './SocialLogin';

interface FormData {
  email: string;
  password: string;
  confirmPassword: string;
  firstName: string;
  lastName: string;
  company: string;
  role: string;
}

const steps = [
  { id: 1, name: 'Compte', description: 'Créez votre compte' },
  { id: 2, name: 'Informations', description: 'Vos informations' },
  { id: 3, name: 'Entreprise', description: 'Détails professionnels' }
];

export function SignupForm() {
  const [currentStep, setCurrentStep] = useState(1);
  const [formData, setFormData] = useState<FormData>({
    email: '',
    password: '',
    confirmPassword: '',
    firstName: '',
    lastName: '',
    company: '',
    role: ''
  });
  const [errors, setErrors] = useState<Partial<FormData>>({});

  const updateFormData = (field: keyof FormData, value: string) => {
    setFormData(prev => ({ ...prev, [field]: value }));
    // Clear error when user starts typing
    if (errors[field]) {
      setErrors(prev => ({ ...prev, [field]: '' }));
    }
  };

  const validateStep = (step: number): boolean => {
    const newErrors: Partial<FormData> = {};

    if (step === 1) {
      if (!formData.email) {
        newErrors.email = 'L\'email est requis';
      } else if (!/\S+@\S+\.\S+/.test(formData.email)) {
        newErrors.email = 'Email invalide';
      }

      if (!formData.password) {
        newErrors.password = 'Le mot de passe est requis';
      } else if (formData.password.length < 8) {
        newErrors.password = 'Minimum 8 caractères';
      }

      if (!formData.confirmPassword) {
        newErrors.confirmPassword = 'Confirmez le mot de passe';
      } else if (formData.password !== formData.confirmPassword) {
        newErrors.confirmPassword = 'Les mots de passe ne correspondent pas';
      }
    }

    if (step === 2) {
      if (!formData.firstName) newErrors.firstName = 'Le prénom est requis';
      if (!formData.lastName) newErrors.lastName = 'Le nom est requis';
    }

    if (step === 3) {
      if (!formData.company) newErrors.company = 'L\'entreprise est requise';
      if (!formData.role) newErrors.role = 'Le rôle est requis';
    }

    setErrors(newErrors);
    return Object.keys(newErrors).length === 0;
  };

  const handleNext = () => {
    if (validateStep(currentStep)) {
      if (currentStep < steps.length) {
        setCurrentStep(currentStep + 1);
      } else {
        handleSubmit();
      }
    }
  };

  const handleBack = () => {
    if (currentStep > 1) {
      setCurrentStep(currentStep - 1);
    }
  };

  const handleSubmit = () => {
    console.log('Form submitted:', formData);
    alert('Inscription réussie ! 🎉');
  };

  return (
    <div className="w-full lg:w-1/2 flex items-center justify-center p-8 bg-white overflow-y-auto">
      <div className="w-full max-w-md">
        {/* Progress Indicator */}
        <div className="mb-8">
          <div className="flex items-center justify-between mb-2">
            {steps.map((step, index) => (
              <div key={step.id} className="flex items-center flex-1">
                <div className="flex flex-col items-center w-full">
                  <div
                    className={`w-10 h-10 rounded-full flex items-center justify-center border-2 transition-all duration-300 ${
                      currentStep > step.id
                        ? 'bg-indigo-600 border-indigo-600 text-white'
                        : currentStep === step.id
                        ? 'bg-indigo-600 border-indigo-600 text-white'
                        : 'bg-white border-gray-300 text-gray-400'
                    }`}
                  >
                    {currentStep > step.id ? (
                      <Check className="w-5 h-5" />
                    ) : (
                      <span>{step.id}</span>
                    )}
                  </div>
                  <div className="text-xs mt-2 text-center">
                    <div className={`font-medium ${currentStep >= step.id ? 'text-gray-900' : 'text-gray-400'}`}>
                      {step.name}
                    </div>
                  </div>
                </div>
                {index < steps.length - 1 && (
                  <div
                    className={`h-0.5 flex-1 -mx-2 transition-all duration-300 ${
                      currentStep > step.id ? 'bg-indigo-600' : 'bg-gray-300'
                    }`}
                    style={{ marginTop: '-2rem' }}
                  />
                )}
              </div>
            ))}
          </div>
        </div>

        {/* Form Header */}
        <div className="mb-8">
          <h2 className="text-3xl font-bold text-gray-900 mb-2">
            {steps[currentStep - 1].description}
          </h2>
          <p className="text-gray-600">
            Étape {currentStep} sur {steps.length}
          </p>
        </div>

        {/* Social Login - Only on Step 1 */}
        {currentStep === 1 && (
          <div className="mb-6">
            <SocialLogin />
            <div className="relative my-6">
              <div className="absolute inset-0 flex items-center">
                <div className="w-full border-t border-gray-300"></div>
              </div>
              <div className="relative flex justify-center text-sm">
                <span className="px-2 bg-white text-gray-500">
                  Ou continuez avec votre email
                </span>
              </div>
            </div>
          </div>
        )}

        {/* Form Steps */}
        <form onSubmit={(e) => { e.preventDefault(); handleNext(); }}>
          <AnimatePresence mode="wait">
            {currentStep === 1 && (
              <motion.div
                key="step1"
                initial={{ opacity: 0, x: 20 }}
                animate={{ opacity: 1, x: 0 }}
                exit={{ opacity: 0, x: -20 }}
                transition={{ duration: 0.3 }}
                className="space-y-4"
              >
                <div>
                  <label htmlFor="email" className="block text-sm font-medium text-gray-700 mb-1">
                    Adresse email
                  </label>
                  <input
                    type="email"
                    id="email"
                    value={formData.email}
                    onChange={(e) => updateFormData('email', e.target.value)}
                    className={`w-full px-4 py-3 border rounded-lg focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-shadow ${
                      errors.email ? 'border-red-500' : 'border-gray-300'
                    }`}
                    placeholder="vous@exemple.com"
                  />
                  {errors.email && (
                    <p className="text-red-500 text-sm mt-1">{errors.email}</p>
                  )}
                </div>

                <div>
                  <label htmlFor="password" className="block text-sm font-medium text-gray-700 mb-1">
                    Mot de passe
                  </label>
                  <input
                    type="password"
                    id="password"
                    value={formData.password}
                    onChange={(e) => updateFormData('password', e.target.value)}
                    className={`w-full px-4 py-3 border rounded-lg focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-shadow ${
                      errors.password ? 'border-red-500' : 'border-gray-300'
                    }`}
                    placeholder="Minimum 8 caractères"
                  />
                  {errors.password && (
                    <p className="text-red-500 text-sm mt-1">{errors.password}</p>
                  )}
                </div>

                <div>
                  <label htmlFor="confirmPassword" className="block text-sm font-medium text-gray-700 mb-1">
                    Confirmer le mot de passe
                  </label>
                  <input
                    type="password"
                    id="confirmPassword"
                    value={formData.confirmPassword}
                    onChange={(e) => updateFormData('confirmPassword', e.target.value)}
                    className={`w-full px-4 py-3 border rounded-lg focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-shadow ${
                      errors.confirmPassword ? 'border-red-500' : 'border-gray-300'
                    }`}
                    placeholder="Confirmez votre mot de passe"
                  />
                  {errors.confirmPassword && (
                    <p className="text-red-500 text-sm mt-1">{errors.confirmPassword}</p>
                  )}
                </div>
              </motion.div>
            )}

            {currentStep === 2 && (
              <motion.div
                key="step2"
                initial={{ opacity: 0, x: 20 }}
                animate={{ opacity: 1, x: 0 }}
                exit={{ opacity: 0, x: -20 }}
                transition={{ duration: 0.3 }}
                className="space-y-4"
              >
                <div>
                  <label htmlFor="firstName" className="block text-sm font-medium text-gray-700 mb-1">
                    Prénom
                  </label>
                  <input
                    type="text"
                    id="firstName"
                    value={formData.firstName}
                    onChange={(e) => updateFormData('firstName', e.target.value)}
                    className={`w-full px-4 py-3 border rounded-lg focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-shadow ${
                      errors.firstName ? 'border-red-500' : 'border-gray-300'
                    }`}
                    placeholder="Jean"
                  />
                  {errors.firstName && (
                    <p className="text-red-500 text-sm mt-1">{errors.firstName}</p>
                  )}
                </div>

                <div>
                  <label htmlFor="lastName" className="block text-sm font-medium text-gray-700 mb-1">
                    Nom
                  </label>
                  <input
                    type="text"
                    id="lastName"
                    value={formData.lastName}
                    onChange={(e) => updateFormData('lastName', e.target.value)}
                    className={`w-full px-4 py-3 border rounded-lg focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-shadow ${
                      errors.lastName ? 'border-red-500' : 'border-gray-300'
                    }`}
                    placeholder="Dupont"
                  />
                  {errors.lastName && (
                    <p className="text-red-500 text-sm mt-1">{errors.lastName}</p>
                  )}
                </div>
              </motion.div>
            )}

            {currentStep === 3 && (
              <motion.div
                key="step3"
                initial={{ opacity: 0, x: 20 }}
                animate={{ opacity: 1, x: 0 }}
                exit={{ opacity: 0, x: -20 }}
                transition={{ duration: 0.3 }}
                className="space-y-4"
              >
                <div>
                  <label htmlFor="company" className="block text-sm font-medium text-gray-700 mb-1">
                    Entreprise
                  </label>
                  <input
                    type="text"
                    id="company"
                    value={formData.company}
                    onChange={(e) => updateFormData('company', e.target.value)}
                    className={`w-full px-4 py-3 border rounded-lg focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-shadow ${
                      errors.company ? 'border-red-500' : 'border-gray-300'
                    }`}
                    placeholder="Nom de votre entreprise"
                  />
                  {errors.company && (
                    <p className="text-red-500 text-sm mt-1">{errors.company}</p>
                  )}
                </div>

                <div>
                  <label htmlFor="role" className="block text-sm font-medium text-gray-700 mb-1">
                    Rôle
                  </label>
                  <select
                    id="role"
                    value={formData.role}
                    onChange={(e) => updateFormData('role', e.target.value)}
                    className={`w-full px-4 py-3 border rounded-lg focus:outline-none focus:ring-2 focus:ring-indigo-500 transition-shadow ${
                      errors.role ? 'border-red-500' : 'border-gray-300'
                    }`}
                  >
                    <option value="">Sélectionnez votre rôle</option>
                    <option value="developer">Développeur</option>
                    <option value="designer">Designer</option>
                    <option value="manager">Manager</option>
                    <option value="ceo">CEO</option>
                    <option value="other">Autre</option>
                  </select>
                  {errors.role && (
                    <p className="text-red-500 text-sm mt-1">{errors.role}</p>
                  )}
                </div>
              </motion.div>
            )}
          </AnimatePresence>

          {/* Navigation Buttons */}
          <div className="flex gap-4 mt-8">
            {currentStep > 1 && (
              <button
                type="button"
                onClick={handleBack}
                className="flex-1 px-6 py-3 border border-gray-300 text-gray-700 rounded-lg hover:bg-gray-50 transition-colors"
              >
                Retour
              </button>
            )}
            <button
              type="submit"
              className={`px-6 py-3 bg-indigo-600 text-white rounded-lg hover:bg-indigo-700 transition-colors ${
                currentStep === 1 ? 'w-full' : 'flex-1'
              }`}
            >
              {currentStep === steps.length ? 'Créer mon compte' : 'Continuer'}
            </button>
          </div>
        </form>

        {/* Login Link */}
        <p className="text-center text-sm text-gray-600 mt-6">
          Vous avez déjà un compte ?{' '}
          <a href="#" className="text-indigo-600 hover:text-indigo-700 font-medium">
            Se connecter
          </a>
        </p>
      </div>
    </div>
  );
}
