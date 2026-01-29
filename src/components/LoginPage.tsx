import { useState } from 'react';
import { Zap, Mail, Lock, User } from 'lucide-react';
import { ImageWithFallback } from './figma/ImageWithFallback';

interface LoginPageProps {
  onLogin: (email: string, password: string) => void;
  onSignup: (name: string, email: string, password: string) => void;
}

export function LoginPage({ onLogin, onSignup }: LoginPageProps) {
  const [isSignup, setIsSignup] = useState(false);
  const [name, setName] = useState('');
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    if (isSignup) {
      onSignup(name, email, password);
    } else {
      onLogin(email, password);
    }
  };

  return (
    <div className="min-h-screen flex">
      {/* Left Panel - Image */}
      <div className="hidden lg:flex lg:w-1/2 relative overflow-hidden">
        <div className="absolute inset-0 bg-gradient-to-br from-green-600 via-emerald-600 to-teal-600 opacity-90 z-10" />
        <ImageWithFallback
          src="https://images.unsplash.com/photo-1593941707874-ef25b8b4a92b?crop=entropy&cs=tinysrgb&fit=max&fm=jpg&ixid=M3w3Nzg4Nzd8MHwxfHNlYXJjaHwxfHxlbGVjdHJpYyUyMHZlaGljbGUlMjBjaGFyZ2luZ3xlbnwxfHx8fDE3Njg0NjIwMjd8MA&ixlib=rb-4.1.0&q=80&w=1080"
          alt="Charging electric vehicle"
          className="w-full h-full object-cover"
        />
        <div className="absolute inset-0 z-20 flex flex-col justify-center items-center text-white p-12">
          <div className="flex items-center gap-3 mb-6">
            <div className="bg-white/20 backdrop-blur-sm p-4 rounded-2xl">
              <Zap className="w-12 h-12" />
            </div>
          </div>
          <h1 className="text-5xl font-bold mb-4 text-center">EV Charge Manager</h1>
          <p className="text-xl text-white/90 text-center max-w-md">
            Gérez facilement la recharge de vos véhicules électriques
          </p>
        </div>
      </div>

      {/* Right Panel - Form */}
      <div className="w-full lg:w-1/2 flex items-center justify-center p-8 bg-white">
        <div className="w-full max-w-md">
          {/* Mobile Logo */}
          <div className="lg:hidden flex items-center justify-center gap-3 mb-8">
            <div className="bg-green-600 p-3 rounded-xl">
              <Zap className="w-8 h-8 text-white" />
            </div>
            <h1 className="text-2xl font-bold text-gray-900">EV Charge</h1>
          </div>

          <div className="mb-8">
            <h2 className="text-3xl font-bold text-gray-900 mb-2">
              {isSignup ? 'Créer un compte' : 'Bienvenue'}
            </h2>
            <p className="text-gray-600">
              {isSignup 
                ? 'Créez votre compte pour commencer' 
                : 'Connectez-vous pour gérer vos recharges'}
            </p>
          </div>

          <form onSubmit={handleSubmit} className="space-y-4">
            {isSignup && (
              <div>
                <label htmlFor="name" className="block text-sm font-medium text-gray-700 mb-1">
                  Nom complet
                </label>
                <div className="relative">
                  <User className="absolute left-3 top-1/2 -translate-y-1/2 w-5 h-5 text-gray-400" />
                  <input
                    type="text"
                    id="name"
                    value={name}
                    onChange={(e) => setName(e.target.value)}
                    className="w-full pl-10 pr-4 py-3 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-green-500"
                    placeholder="Votre nom"
                    required
                  />
                </div>
              </div>
            )}

            <div>
              <label htmlFor="email" className="block text-sm font-medium text-gray-700 mb-1">
                Adresse email
              </label>
              <div className="relative">
                <Mail className="absolute left-3 top-1/2 -translate-y-1/2 w-5 h-5 text-gray-400" />
                <input
                  type="email"
                  id="email"
                  value={email}
                  onChange={(e) => setEmail(e.target.value)}
                  className="w-full pl-10 pr-4 py-3 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-green-500"
                  placeholder="vous@exemple.com"
                  required
                />
              </div>
            </div>

            <div>
              <label htmlFor="password" className="block text-sm font-medium text-gray-700 mb-1">
                Mot de passe
              </label>
              <div className="relative">
                <Lock className="absolute left-3 top-1/2 -translate-y-1/2 w-5 h-5 text-gray-400" />
                <input
                  type="password"
                  id="password"
                  value={password}
                  onChange={(e) => setPassword(e.target.value)}
                  className="w-full pl-10 pr-4 py-3 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-green-500"
                  placeholder="••••••••"
                  required
                />
              </div>
            </div>

            {!isSignup && (
              <div className="flex items-center justify-between text-sm">
                <label className="flex items-center">
                  <input type="checkbox" className="mr-2 rounded" />
                  <span className="text-gray-600">Se souvenir de moi</span>
                </label>
                <a href="#" className="text-green-600 hover:text-green-700">
                  Mot de passe oublié ?
                </a>
              </div>
            )}

            <button
              type="submit"
              className="w-full bg-green-600 text-white py-3 rounded-lg hover:bg-green-700 transition-colors font-medium"
            >
              {isSignup ? 'Créer mon compte' : 'Se connecter'}
            </button>
          </form>

          <p className="text-center text-sm text-gray-600 mt-6">
            {isSignup ? 'Vous avez déjà un compte ?' : "Vous n'avez pas de compte ?"}{' '}
            <button
              onClick={() => setIsSignup(!isSignup)}
              className="text-green-600 hover:text-green-700 font-medium"
            >
              {isSignup ? 'Se connecter' : "S'inscrire"}
            </button>
          </p>
        </div>
      </div>
    </div>
  );
}
